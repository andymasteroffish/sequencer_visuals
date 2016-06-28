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
    
    publicRelease = false;
    
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
    stepModeIcons.whiteVal = whiteVal;
    
    showHelp = false;
    
    bpm.reset();
    bpmStartValue = 160;
    bpmValue = bpmStartValue;
    bpm.setBpm(bpmValue);
    bpm.setBeatPerBar(8);
    bpm.prcForPreHit = 0.9;
    ofAddListener(bpm.beatEvent, this, &Sequencer::hitBeat);
    ofAddListener(bpm.preBeatEvent, this, &Sequencer::preHitBeat);
    bpm.start();
    
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
    
    //set the markers
    beatXSpacing = 40;
    beatYDistFromBottom = 40;
    float beatXPadding = (ofGetWidth()-(beatXSpacing*NUM_BEATS))/2;
    for (int i=0; i<NUM_BEATS; i++){
        beatMarkers[i].setup(beatXPadding+beatXSpacing*i, ofGetHeight()-beatYDistFromBottom);
    }
    
    //fucking aorund
    visualEffectNum = 0;
    setVisualEffect();
    
    //setting up iOS
#ifdef USING_IOS
    int buttonW = ofGetWidth()/8;
    int buttonH = ofGetHeight()/2;
    for (int i=0; i<NUM_TOUCH_BUTTONS; i++){
        int xPos = (i%8) * buttonW;
        int yPos = (i/8) * buttonH;
        touchButtons[i].setup(xPos, yPos, buttonW, buttonH);
    }
    
    //the tep mode buttons
    int stepButtonW = ofGetWidth() / NUM_TUOUCH_STEP_BUTTONS;
    for (int i=0; i<NUM_TUOUCH_STEP_BUTTONS; i++){
        touchStepButtons[i].setup(i*stepButtonW, 0, stepButtonW, ofGetHeight());
    }
#endif
    
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
    
    //update the markers
    for (int i=0; i<NUM_BEATS; i++){
        beatMarkers[i].update(deltaTime);
    }
    
    //update ios buttons
#ifdef USING_IOS
    for (int i=0; i<NUM_TOUCH_BUTTONS; i++){
        touchButtons[i].update(deltaTime);
    }
    for (int i=0; i<NUM_TUOUCH_STEP_BUTTONS; i++){
        touchStepButtons[i].update(deltaTime);
    }
#endif
    
    //testing
//    for (int i=0; i<NUM_IOS_BEATS_PER_SOUND; i++){
//        cout<<i<<"  "<<sounds[i][14].getPosition()<<endl;
//    }
}

//--------------------------------------------------------------
void Sequencer::draw(){
    
    //iOS buttons
#ifdef USING_IOS
    ofEnableAlphaBlending();
    if (!stepMode){
        for (int i=0; i<NUM_TOUCH_BUTTONS; i++){
            touchButtons[i].draw();
        }
    }else{
        for (int i=0; i<NUM_TUOUCH_STEP_BUTTONS; i++){
            touchStepButtons[i].draw();
        }
    }
    ofDisableAlphaBlending();
#endif
    
    
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
    
    //are we in step mode?
    if (stepMode){
        //draw the current icon
        ofSetColor(0);
        ofPushMatrix();
        ofTranslate(beatMarkers[0].pos.x - 80, beatMarkers[0].pos.y);
        stepModeIcons.draw(curStepSound);
        ofPopMatrix();
        //ofDrawBitmapString(ofToString(curStepSound), beatMarkers[0].pos.x - 70, beatMarkers[0].pos.y+10);
    }
    
    if (!publicRelease){
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
    //cout<<"key "<<key<<endl;
    if (key == 'h' || key == 'H'){
        showHelp = !showHelp;
    }
    if (key == 's'){
        stepMode = !stepMode;
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
    if (key == 'n'){
        useNumpadKeys = !useNumpadKeys;
    }
    if (key == 'v'){
        setVisualEffect();
    }
    
    if (key == ' '){
        recording = !recording;
    }
    
    if (key == OF_KEY_UP){
        bpmValue += 10;
        bpmValue = MIN(bpmValue, 400);
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
            recording = !recording;
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
void Sequencer::touchDown(ofTouchEventArgs & touch){

    
    //check the buttons!
    if (!stepMode){
        for (int i=0; i<NUM_TOUCH_BUTTONS; i++){
            if(touchButtons[i].checkHit(touch.x, touch.y)){
                if (i < 15){
                    makeNewHit(i);
                }
                if (i == 15){
                    clearBeats();
                }
            }
        }
    }else{
        for (int i=0; i<NUM_TUOUCH_STEP_BUTTONS; i++){
            if (touchButtons[i].checkHit(touch.x, touch.y)){
                
                cout<<"mother fuck "<<i<<endl;
                
                if (i==0){
                    curStepSound--;
                    if (curStepSound < 0)   curStepSound = NUM_BEATS-1;
                }
                else if (i==NUM_TUOUCH_STEP_BUTTONS-1){
                    curStepSound++;
                    if (curStepSound >= NUM_BEATS)   curStepSound = 0;
                }
                else{
                    stepModePress(i-1);
                }
                
            }
        }
    }
}


//--------------------------------------------------------------
void Sequencer::windowResized(int w, int h){
    //set the markers
    float beatXPadding = (ofGetWidth()-(beatXSpacing*NUM_BEATS))/2;
    for (int i=0; i<NUM_BEATS; i++){
        beatMarkers[i].pos.set(beatXPadding+beatXSpacing*i, ofGetHeight()-beatYDistFromBottom);
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
        
        thisHit->setup(ofGetWidth(), ofGetHeight(), whiteVal);
        
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
void Sequencer::stepModePress(int placeID){
    beatsOn[placeID][curStepSound] = !beatsOn[placeID][curStepSound];
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
        recording = true;
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