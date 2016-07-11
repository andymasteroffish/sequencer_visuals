//
//  Sequencer.cpp
//  sequencer
//
//  Created by Andrew Wallace on 6/28/16.
//
//

#include "Sequencer.hpp"

//--------------------------------------------------------------
void Sequencer::setup(){
    
#ifdef TARGET_OPENGLES
    shader.load("shaders/shadersES2/shader");
#else
    if(ofIsGLProgrammableRenderer()){
        shader.load("shaders/shadersGL3/shader");
        
    }else{
        shader.load("shaders/shadersGL2/shader");
        //this is the one my computer uses
    }
#endif
    
    cout<<ofGetWidth()<<" x "<<ofGetHeight()<<endl;
    
    //the iPad screen is bug enough that we really need to scale everything up
    usingIPad = ofGetWidth() > 2000;
    
    gameW = ofGetWidth();
    gameH = ofGetHeight();
    
    if (usingIPad){
        cout<<"it ipad"<<endl;
        gameW = ofGetWidth()/2;
        gameH = ofGetHeight()/2;
    }
    
    
    publicRelease = true;
    
    useNumpadKeys = false;
    usePreHitDetection = true;
    
    autoPlay = false;
    recording = true;
    turnOnRecordingWhenClearing = true;
    
    stepMode = false;
    curStepSound = 0;
    
    ofSetFrameRate(60);
    
    whiteVal = 240;
    ofBackground(whiteVal);
    
    if (publicRelease){
        ofToggleFullscreen();
    }
    
    showHelp = false;
    showTouchButtons = true;
    
    bpmValue = 160;
    bpmStartValue = bpmValue;
    
    ofAddListener(bpm.beatEvent, this, &Sequencer::hitBeat);
    ofAddListener(bpm.preBeatEvent, this, &Sequencer::preHitBeat);
    bpm.start(bpmValue);
    bpm.setPreHitPrcSpacing(0.5);
    
//    bpm.reset();
//    bpmStartValue = 160;
//    bpmValue = bpmStartValue;
//    bpm.setBpm(bpmValue);
//    bpm.setBeatPerBar(8);
//    bpm.prcForPreHit = 0.9;
//    ofAddListener(bpm.beatEvent, this, &Sequencer::hitBeat);
//    ofAddListener(bpm.preBeatEvent, this, &Sequencer::preHitBeat);
//    bpm.start();
    
    thisBeat = 0;
    onPreHit = false;
    
    prevFrameTime = ofGetElapsedTimef();
    deltaTime = 0;
    
    //set the sounds
#ifdef USING_IOS
    loadSounds("sound_source.txt");
#else
    loadSounds("../sound_source.txt");
#endif
    
    clearBeats();
    
    //fonts
    buttonFont.load("Futura.ttf", 25);
    
    aboutScreen.setup(whiteVal, usingIPad);
    
    //set the markers
    beatXSpacing = 40;
    beatYDistFromBottom = 40;
    if (usingIPad){
        beatXSpacing *= 2;
        beatYDistFromBottom *= 2;
    }
    
    for (int i=0; i<NUM_SOUNDS; i++){
        soundButtons[i].setup(i, whiteVal);
    }
    
    //fucking aorund
    visualEffectNum = 0;
    setVisualEffect();
    
    //setting the locaitonof buttons based on screen size
    setButtonPositions();
    
    string text[NUM_TOUCH_MENU_BUTTONS] = {"Live", "Step", "Clear", "-", "+"};
    for (int i=0; i<NUM_TOUCH_MENU_BUTTONS; i++){
        touchMenuButtons[i].setText(text[i], &buttonFont);
    }
}

//--------------------------------------------------------------
void Sequencer::preHitBeat(void){
    //cout<<"pre hit!!"<<endl;
    if (usePreHitDetection){
        onPreHit = true;
    }
}
//--------------------------------------------------------------
void Sequencer::hitBeat(void){
    onPreHit = false;
    thisBeat++;
    if (thisBeat >= NUM_BEATS){
        thisBeat = 0;
    }
    
    //cout<<"hit beat "<<thisBeat<<endl;
    
    //play anything that's on
    for (int k=0; k<NUM_SOUNDS; k++){
        if (beatsOn[thisBeat][k]){
            makeNewHit(k);
        }
    }
    
    beatMarkers[thisBeat].triggerBeat();
    
    if (autoPlay && ofRandomuf() < 0.2){
        makeNewHit(ofRandom(15));
    }
}

//--------------------------------------------------------------
void Sequencer::update(){
    
    deltaTime = ofGetElapsedTimef() - prevFrameTime;
    prevFrameTime = ofGetElapsedTimef();
    
    for (int i=hits.size()-1; i>=0; i--){
        hits[i]->update(deltaTime);
        if (hits[i]->killMe){
            hits[i]->cleanUp();
            delete hits[i];
            hits.erase(hits.begin()+i);
        }
    }
    
    aboutScreen.update(deltaTime);
    
    //update the markers
    for (int i=0; i<NUM_BEATS; i++){
        beatMarkers[i].update(deltaTime, stepMode);
    }
    
    //update ios buttons
//#ifdef USING_IOS
    for (int i=0; i<NUM_TOUCH_BUTTONS; i++){
        touchButtons[i].update(deltaTime);
    }
    for (int i=0; i<NUM_TOUCH_MENU_BUTTONS; i++){
        //slide them to hide "Live/Record" during step mode
        float targetY = menuButtonH * i;
        if (stepMode)   targetY = menuButtonH * (i-1);
        
        if (i==NUM_TOUCH_MENU_BUTTONS-1){    //both tempo buttons should be on the same Y plane
            targetY -= menuButtonH;
        }
        
        float xeno = 0.85;
        touchMenuButtons[i].box.y = xeno * touchMenuButtons[i].box.y + (1-xeno) * targetY;
        
        //actually update them
        touchMenuButtons[i].update(deltaTime);
    }
    aboutButton.update(deltaTime);
    for (int i=0; i<NUM_BEATS; i++){
        touchStepButtons[i].update(deltaTime);
    }
    
    for (int i=0; i<NUM_SOUNDS; i++){
        soundButtons[i].update(deltaTime, curStepSound);
    }
//#endif
    
    //testing
//    for (int i=0; i<NUM_IOS_BEATS_PER_SOUND; i++){
//        cout<<i<<"  "<<sounds[i][14].getPosition()<<endl;
//    }
}

//--------------------------------------------------------------
void Sequencer::draw(){
    
    //touch buttons
    if (showTouchButtons){
        ofEnableAlphaBlending();
        if (!stepMode){
            for (int i=0; i<NUM_TOUCH_BUTTONS; i++){
                touchButtons[i].draw();
            }
        }else{
            for (int i=0; i<NUM_BEATS; i++){
                touchStepButtons[i].draw();
            }
        }
        
        for (int i=0; i<NUM_TOUCH_MENU_BUTTONS; i++){
            touchMenuButtons[i].draw();
        }
        
        //help button
        ofFill();
        ofSetColor(whiteVal);
        ofDrawRectangle(aboutButton.box);
        aboutButton.draw();
        
        //write the bpm
        ofSetColor(0);
        string bpmText = ofToString( (int)bpmValue);
        float textW = buttonFont.stringWidth(bpmText);
        ofPushMatrix();
        ofTranslate(touchMenuButtons[3].box.x+touchMenuButtons[3].box.width, touchButtons[3].box.y+touchButtons[3].box.height-10);
        float bpmTextScale = usingIPad ? 1 : 0.5;
        ofScale(bpmTextScale,bpmTextScale);
        buttonFont.drawString(bpmText, -textW/2, 0);
        ofPopMatrix();
        
        for (int i=0; i<NUM_SOUNDS; i++){
            soundButtons[i].draw();
        }
        ofDisableAlphaBlending();
    }
    
    
    //shader stuff
    shader.begin();
    
    shader.setUniform1f("time", ofGetElapsedTimef());
    
    if (doDisplacement){
        shader.setUniform1f("displacementHeight", sin(ofGetElapsedTimef()*0.3) * 70);
    }else{
        shader.setUniform1f("displacementHeight", 0);
    }
    
    shader.setUniform3f("colA", colorA.r/255.0f, colorA.g/255.0f, colorA.b/255.0f);
    if (doColorFade){
        shader.setUniform3f("colB", colorB.r/255.0f, colorB.g/255.0f, colorB.b/255.0f);
    }else{
        //use the same color for colB
        shader.setUniform3f("colB", colorA.r/255.0f, colorA.g/255.0f, colorA.b/255.0f);
    }
    
    if (doCamMovement){
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        float rotationRange = 50;
        float noiseSpeed = 0.075;
        float xRot = (1- 2*ofNoise( ofGetElapsedTimef() * noiseSpeed, 0)) * rotationRange;
        float yRot = (1- 2*ofNoise( ofGetElapsedTimef() * noiseSpeed, 100)) * rotationRange;
        ofRotateX(xRot);
        ofRotateY(yRot);
        ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
    }
    
    //dealing with blowing things up on iPad
    if (usingIPad){
        ofPushMatrix();
        ofScale(2, 2);
    }
    
    for (int i=0; i<hits.size(); i++){
        if (doCamMovement){
            ofPushMatrix();
            ofTranslate(0,0, hits[i]->zVal);
        }
        
        hits[i]->draw();
        
        if (doCamMovement){
            ofPopMatrix();
        }
    }
    
    if (usingIPad){
        ofPopMatrix();
    }
    
    if (doCamMovement){
        ofPopMatrix();
    }
    
    
    //draw the markers
    for (int i=0; i<NUM_BEATS; i++){
        bool anyOn = false;
        if (!stepMode){
            for (int k=0; k<NUM_SOUNDS; k++){
                if (beatsOn[i][k])  anyOn = true;
            }
        }else{
            anyOn = beatsOn[i][curStepSound];
        }
        
        beatMarkers[i].draw(anyOn, recording);
    }
    
    shader.end();
    
    aboutScreen.draw();
    
    bool showWIPText = !publicRelease;
#ifdef USING_IOS
    showWIPText = false;
#endif
    if (showWIPText){
        ofSetColor(0);
        string text =   "Sequencer by Andy Wallace - andy@andymakes.com";
        text +=         "\nWork in progress. Please do not distribute.";
        text +=         "\nPress h for help.";
        ofDrawBitmapString(text, 10, 15);
    }
    
    if (showHelp){
        ofSetColor(0);
        string text = "";
        if (!useNumpadKeys){
            if (!stepMode){
                text += "keys 1-0 & q-t for sounds\n";
            }else{
                text += "keys 1-0 & q-y to toggle current step mode sound\n";
            }
        }else{
            text += "most numpad keys for sound (including non-numbers)\n";
        }
        text += "return to clear\n";
        if (!useNumpadKeys){
            text += "space to start/stop recording\n";
            text += "v to toggle visual effects\n";
        }else{
            text += "0 to start/stop recording\n";
            text += ". to toggle visual effects\n";
        }
        text += "\n";
        text += "up & down to change BPM (currently "+ofToString(bpmValue)+")\n";
        text += "b to reset BPM\n";
        text += "\n";
        text += "f to toggle full screen\n";
        text += "\n";
        string stepModeStatus = (stepMode ? "on" : "off");
        text += "s to toggle step mode (currently "+stepModeStatus+")\n";
        text += "left and right to change sound when in step mode\n";
        text += "currently step mode is keyboard mode only (no numpad)\n";
        text += "\n";
        string turnOnRecordStatus = (turnOnRecordingWhenClearing ? "on" : "off");
        text += "c to toggle turning on record when clearing (currently "+turnOnRecordStatus+")\n";
        string preHitStatus = (usePreHitDetection ? "on" : "off");
        text += "p to toggle pre hit detection (currently "+preHitStatus+")\n";
        string autoStatus = (autoPlay ? "on" : "off");
        text += "a for auto play (currently "+autoStatus+")\n";
        text += "\n";
        string numPadStatus = (useNumpadKeys ? "on" : "off");
        text += "n to toggle numpad key mapping (currently "+numPadStatus+")\n";
        text += "\n";
        text += "h to hide this\n";
        
        ofDrawBitmapString(text, 10, 90);
    }
    
    
}


//--------------------------------------------------------------
void Sequencer::keyPressed(int key){
    
    if (aboutScreen.isActive){
        aboutScreen.turnOff();
        return;
    }
    
    //cout<<"key "<<key<<endl;
    if (key == 'h' || key == 'H'){
        showTouchButtons = !showTouchButtons;
        //showHelp = !showHelp;
    }
    if (key == 'z' || key == 'Z'){
        aboutScreen.turnOn();
    }
    
    if (key == 's'){
        setStepMode(!stepMode);
    }
    if (key == 'a'){
        autoPlay = !autoPlay;
    }
    if (key == 'f'){
        ofToggleFullscreen();
    }
    if (key == 'c'){
        turnOnRecordingWhenClearing = !turnOnRecordingWhenClearing;
    }
    if (key == 'p'){
        usePreHitDetection = !usePreHitDetection;
    }
    if (key == '/'){
        useNumpadKeys = !useNumpadKeys;
    }
    if (key == 'v'){
        setVisualEffect();
    }
    
    if (key == ' '){
        setRecording(!recording);
    }
    
    if (key == OF_KEY_UP){
        bpmValue += 10;
        bpmValue = MIN(bpmValue, 500);
        bpm.setBpm(bpmValue);
    }
    if (key == OF_KEY_DOWN){
        bpmValue -= 10;
        bpmValue = MAX(bpmValue, 50);
        bpm.setBpm(bpmValue);
    }
    if (key == 'b'){
        bpmValue = bpmStartValue;
        bpm.setBpm(bpmValue);
    }
    
    if (useNumpadKeys){
        if (key == 63289)   makeNewHit(0);  //num lock
        if (key == '/')     makeNewHit(1);
        if (key == '*')     makeNewHit(2);
        if (key == 127)     makeNewHit(3);  //backspace
        
        if (key == '7')     makeNewHit(4);
        if (key == '8')     makeNewHit(5);
        if (key == '9')     makeNewHit(6);
        if (key == '-')     makeNewHit(7);
        
        if (key == '4')     makeNewHit(8);
        if (key == '5')     makeNewHit(9);
        if (key == '6')     makeNewHit(10);
        if (key == '+')     makeNewHit(11);
        
        if (key == '1')     makeNewHit(12);
        if (key == '2')     makeNewHit(13);
        if (key == '3')     makeNewHit(14);
        
        if (key == '0') {
            setRecording(!recording);
        }
        
        if (key == 13){  //Enter
            clearBeats();
        }
        
        if (key == '.'){  //del
            setVisualEffect();
        }
    }
    
    //laptop control
    if (!useNumpadKeys){
        if (!stepMode){
            if (key == '0')     makeNewHit(0);
            if (key == '1')     makeNewHit(1);
            if (key == '2')     makeNewHit(2);
            if (key == '3')     makeNewHit(3);
            if (key == '4')     makeNewHit(4);
            if (key == '5')     makeNewHit(5);
            if (key == '6')     makeNewHit(6);
            if (key == '7')     makeNewHit(7);
            if (key == '8')     makeNewHit(8);
            if (key == '9')     makeNewHit(9);
            if (key == 'q')     makeNewHit(10);
            if (key == 'w')     makeNewHit(11);
            if (key == 'e')     makeNewHit(12);
            if (key == 'r')     makeNewHit(13);
            if (key == 't')     makeNewHit(14);
        }else{
            if (key == '1')     stepModePress(0);
            if (key == '2')     stepModePress(1);
            if (key == '3')     stepModePress(2);
            if (key == '4')     stepModePress(3);
            if (key == '5')     stepModePress(4);
            if (key == '6')     stepModePress(5);
            if (key == '7')     stepModePress(6);
            if (key == '8')     stepModePress(7);
            if (key == '9')     stepModePress(8);
            if (key == '0')     stepModePress(9);
            if (key == 'q')     stepModePress(10);
            if (key == 'w')     stepModePress(11);
            if (key == 'e')     stepModePress(12);
            if (key == 'r')     stepModePress(13);
            if (key == 't')     stepModePress(14);
            if (key == 'y')     stepModePress(15);
            
            if (key == OF_KEY_LEFT){
                curStepSound--;
                if (curStepSound < 0)   curStepSound = NUM_SOUNDS-1;
            }
            if (key == OF_KEY_RIGHT){
                curStepSound++;
                if (curStepSound >= NUM_SOUNDS) curStepSound = 0;
            }
        }
        
        if (key == 13){  //Enter
            clearBeats();
        }
    }
    
    
    
    
}

//--------------------------------------------------------------
void Sequencer::touchDown(int x, int y){
    
    //if the about screen is on, any touch should dismiss it
    if (aboutScreen.isActive){
        aboutScreen.turnOff();
        return;
    }
    
    //help button overrides anything else
    if (aboutButton.checkHit(x, y) && !aboutScreen.isActive){
        aboutScreen.turnOn();
        return;
    }
    
    //check the buttons!
    if (!stepMode){
        for (int i=0; i<NUM_TOUCH_BUTTONS; i++){
            if(touchButtons[i].checkHit(x, y)){
                if (i < 15){
                    makeNewHit(i);
                }
                if (i == 15){
                    clearBeats();
                }
            }
        }
    }else{
        for (int i=0; i<NUM_BEATS; i++){
            if (touchStepButtons[i].checkHit(x, y)){
               stepModePress(i);
            }
        }
        
        for (int i=0; i<NUM_SOUNDS; i++){
            if (soundButtons[i].checkHit(x, y)){
                curStepSound = i;
            }
        }
    }
    
    //menu buttons
    for (int i=0; i<NUM_TOUCH_MENU_BUTTONS; i++){
        if (touchMenuButtons[i].checkHit(x, y)){
            
            if (i == 0){
                setRecording(!recording);
            }
            
            if (i == 1){
                setStepMode(!stepMode);
            }
            
            if (i == 2){
                clearBeats();
            }
            
            if (i == 3){
                bpmValue -= 10;
                bpmValue = MAX(bpmValue, 50);
                bpm.setBpm(bpmValue);
            }
            if (i == 4){
                bpmValue += 10;
                bpmValue = MIN(bpmValue, 500);
                bpm.setBpm(bpmValue);
            }
        }
    }
    
}


//--------------------------------------------------------------
void Sequencer::windowResized(int w, int h){
    gameW = ofGetWidth();
    gameH = ofGetHeight();
    
    setButtonPositions();
}

//--------------------------------------------------------------
void Sequencer::setButtonPositions(){
    
    float beatXPadding = beatXSpacing/2 + (ofGetWidth()-(beatXSpacing*NUM_BEATS))/2;
    float stepModeBeatSpacing = ofGetWidth()/NUM_BEATS;
    for (int i=0; i<NUM_BEATS; i++){
        float normX = beatXPadding+beatXSpacing*i;
        float stepX = stepModeBeatSpacing/2 + stepModeBeatSpacing*i;
        beatMarkers[i].setup(normX, stepX, ofGetHeight()-beatYDistFromBottom, usingIPad);
    }
    
    int buttonW = ofGetWidth()/8;
    int buttonH = ofGetHeight()/2;
    for (int i=0; i<NUM_TOUCH_BUTTONS; i++){
        int col = i%8;
        int row = i/8;
        
        //keep top right open
        if (col == 7 && row == 0){
            row = 1;
        }
        
        touchButtons[i].setup(col * buttonW, row * buttonH, buttonW, buttonH);
    }
    
    //the three menu buttons fo in the top right
    menuButtonH = buttonH / 4;
    for (int i=0; i<NUM_TOUCH_MENU_BUTTONS; i++){
        string oldText = touchMenuButtons[i].text;
        ofTrueTypeFont * oldFont = touchMenuButtons[i].font;
        if (i < 3){
            touchMenuButtons[i].setup(ofGetWidth() - buttonW, i * menuButtonH, buttonW, menuButtonH);
        }else{
            float tempoX = i==3 ? ofGetWidth() - buttonW : ofGetWidth() - buttonW/2;
            touchMenuButtons[i].setup(tempoX, 3 * menuButtonH, buttonW/2, menuButtonH);
        }
        touchMenuButtons[i].setText(oldText, oldFont);
    }
    
    int aboutButtonW = touchMenuButtons[4].box.width/2;
    int aboutButtonH = touchMenuButtons[4].box.height/2;
    
#ifdef USING_IOS
    aboutButtonW = touchMenuButtons[4].box.width;
    aboutButtonH = touchMenuButtons[4].box.height;
#endif
    
    aboutButton.setup(0, ofGetHeight()/2-aboutButtonH/2, aboutButtonW, aboutButtonH);
    aboutButton.setText("?", &buttonFont);
    
    
    //sound buttons for step mode
    float soundSpacing = (ofGetWidth()-buttonW) / NUM_SOUNDS;
    for (int i=0; i<NUM_SOUNDS; i++){
        soundButtons[i].setPosition(soundSpacing*i, 10, soundSpacing, soundSpacing);
    }
    
    //the step mode buttons
    int stepButtonW = ofGetWidth() / NUM_BEATS;
    for (int i=0; i<NUM_BEATS; i++){
        int yPos = soundSpacing + 50;   //give it a bit of extra spacing
        if (i>=NUM_BEATS-2){
            yPos = ofGetHeight()/2 - menuButtonH;
        }
        touchStepButtons[i].setup(i*stepButtonW, yPos, stepButtonW, ofGetHeight());
    }
}

//--------------------------------------------------------------
void Sequencer::makeNewHit(int idNum){
    //assume we'll play the sound
    bool playSound = true;
    
    //turning this beat on
    if (recording){
        int beatPos = thisBeat;
        if (onPreHit){
            playSound = false;  //we'll catch it in a second
            beatPos = (thisBeat+1)%NUM_BEATS;
        }
        beatsOn[beatPos][idNum] = true;
    }
    
    //Making the hit
    if (playSound){
        Hit * thisHit;
        
        if (idNum == 0)     thisHit = new TunnelHit();
        if (idNum == 1)     thisHit = new SweepHit();
        if (idNum == 2)     thisHit = new TriangleHit();
        if (idNum == 3)     thisHit = new GrapesHit();
        if (idNum == 4)     thisHit = new BuckshotHit();
        if (idNum == 5)     thisHit = new ChaserHit();
        if (idNum == 6)     thisHit = new SlashHit();
        if (idNum == 7)     thisHit = new SquareHit();
        if (idNum == 8)     thisHit = new TrapezoidHit();
        if (idNum == 9)     thisHit = new DotPolygonHit();
        if (idNum == 10)     thisHit = new SizzleHit();
        if (idNum == 11)     thisHit = new DrunkTriangleHit();
        if (idNum == 12)     thisHit = new CrystalHit();
        if (idNum == 13)     thisHit = new ClapHit();
        if (idNum == 14)     thisHit = new WaveColumnHit();
        
        thisHit->setup(gameW, gameH, whiteVal, usingIPad);
        
        hits.push_back(thisHit);
    }
    
    //making the sound
    if (playSound){
#ifdef USING_IOS
        sounds[ thisBeat%NUM_IOS_BEATS_PER_SOUND ][idNum].play();
#else
        sounds[0][idNum].play();
#endif
    }
    
}

//--------------------------------------------------------------
void Sequencer::setStepMode(bool isOn){
    stepMode = isOn;
    for (int i=0; i<NUM_SOUNDS; i++){
        if(stepMode){
            soundButtons[i].stepModeOn();
            setRecording(true);
        }else{
            soundButtons[i].stepModeOff();
        }
    }
    
    touchMenuButtons[1].text = stepMode ? "Normal" : "Step";
}

//--------------------------------------------------------------
void Sequencer::stepModePress(int placeID){
    beatsOn[placeID][curStepSound] = !beatsOn[placeID][curStepSound];
}

//--------------------------------------------------------------
void Sequencer::setRecording(bool isOn){
    recording = isOn;
    touchMenuButtons[0].text = recording ? "Live" : "Record";
}

//--------------------------------------------------------------
void Sequencer::clearBeats(){
    for (int i=0; i<NUM_BEATS; i++){
        for (int k=0; k<NUM_SOUNDS; k++){
            beatsOn[i][k] = false;
        }
        beatMarkers[i].triggerClear();
    }
    if (turnOnRecordingWhenClearing){
        setRecording(true);
    }
}


//--------------------------------------------------------------
void Sequencer::loadSounds(string filePath){
    
    ofBuffer buffer = ofBufferFromFile(filePath);
    vector<string> files;
    
    if (buffer.size() > 0){
        
        for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it){
            
            string line = *it;
            
            if (line.size() > 0){
                if (line[0] != ';'){
                    files.push_back(line);
                }
            }
            
        }
        
        cout<<"found "<<files.size()<<" sound files"<<endl;
        
        for (int i=0; i<NUM_SOUNDS; i++){
#ifdef USING_IOS
            for (int k=0; k<NUM_IOS_BEATS_PER_SOUND; k++){
                sounds[k][i].load(files[i%files.size()]);
            }
#else
            sounds[0][i].load(files[i%files.size()]);
            sounds[0][i].setMultiPlay(true);
#endif
        }
        
    }else{
        cout<<"bad file"<<endl;
    }
}

//--------------------------------------------------------------
void Sequencer::setVisualEffect(){
    visualEffectNum++;
    
    //assume everything will be off
    doDisplacement = false;
    doColorFade = false;
    doCamMovement = false;
    
    //only turn things on every other time, so that every other tap, everything is normal
    if (visualEffectNum % 2 == 0){
        //keep doing until something is on
        while(!doDisplacement && !doColorFade && !doCamMovement){
            doDisplacement = ofRandomuf() < 0.5;
            doColorFade = ofRandomuf() < 0.5;
            doCamMovement = ofRandomuf() < 0.5;
        }
    }
    
    if (doColorFade){
        //make a dark background
        ofBackground(10, 10, 10);
        
        //create some light colors
        colorA.setHsb(ofRandom(255), 255, 255);
        colorB.setHsb(ofRandom(255), 255, 255);
    }else{
        ofBackground(whiteVal);
        colorA.set(0);
        colorB.set(0);
    }
    
}