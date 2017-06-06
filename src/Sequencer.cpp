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
    
    
    publicRelease = false;
    
    useClickTrack = false;
    
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
    
    //maxim - https://github.com/micknoise/Maximilian
    sampleRate 	= 44100;
    bufferSize	= 512; /* Buffer Size. you have to fill this buffer with sound using the for loop in the audioOut method */
    
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    
    //the iPad screen is bug enough that we really need to scale everything up
    usingIPad = ofGetWidth() > 2000;
    
    gameW = ofGetWidth();
    gameH = ofGetHeight();
    
    if (usingIPad){
        cout<<"it an ipad"<<endl;
        gameW = ofGetWidth()/2;
        gameH = ofGetHeight()/2;
    }
    
    hasRunStepMode = checkHasRunStepMode();
    
    isFirstRun = checkIsFirstRun();
    //testing
    if (!publicRelease){
//        isFirstRun = true;
//        hasRunStepMode = false;
    }
    firstRunTime = 30;
    firstRunTimer =  isFirstRun ? firstRunTime : -1;
    hasAddedANote = false;
    
    useNumpadKeys = false;
    usePreHitDetection = true;
    
    autoPlay = false;
    recording = true;
    turnOnRecordingWhenClearing = false;//true;
    
    stepMode = false;
    curStepSound = 0;
    
    //curMouseOverSound = -1;
    
    ofSetFrameRate(60);
    
    whiteVal = 240;
    ofBackground(whiteVal);
    
    //this feels suspicious for iOS. Shoud this only happen on computer?
    if (publicRelease){
        ofToggleFullscreen();
    }
    
    logo.setup(whiteVal, usingIPad, &buttonFont, &buttonFontSmall);
    
    
    showHelp = false;
    showTouchButtons = true;
    
    bpmValue = 160;
    bpmStartValue = bpmValue;
    
    preHitPrc = 0.85f;   //how far through do we have to be for it tocount as a prehit for the next beat
    
    thisBeat = -1;
    onPreHit = false;
    
    prevFrameTime = ofGetElapsedTimef();
    deltaTime = 0;
    
    //set the sounds
    loadSounds("sound_source.txt");
    
    //adjust the click track volume
    //float clickVol = 0.25;//0.45;
    //clickTrackSound.setVolume(clickVol);
    //clickTrackSound2.setVolume(clickVol);
    
    clearBeats();
    
    //fonts
    float ipadAdjust = usingIPad ? 2 : 1;
    buttonFont.load("Futura.ttf", 25 * ipadAdjust);
    buttonFontSmall.load("Futura.ttf", 15 * ipadAdjust);
    
#ifdef USING_IOS
    aboutButtonIcon.load("questionmark_ios.png");
#else
    aboutButtonIcon.load("questionmark.png");
#endif
    
    aboutScreen.setup(whiteVal, usingIPad, false);
    
    if (!hasRunStepMode){
        stepModeInstructions.setup(whiteVal, usingIPad, true);
    }
    
    //set the markers
    beatXSpacing = 50;
    beatYDistFromBottom = 50;
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
    
    string text[NUM_TOUCH_MENU_BUTTONS];
    text[MENU_BUTTON_LIVE] = "";        //filled in later
    text[MENU_BUTTON_STEP] = "";        //filled in later
    text[MENU_BUTTON_CLEAR] = "Clear";
    text[MENU_BUTTON_CLICK] = "";       //filled in later
    text[MENU_BUTTON_TEMPO_DOWN] = "-";
    text[MENU_BUTTON_TEMPO_UP] = "+";
    for (int i=0; i<NUM_TOUCH_MENU_BUTTONS; i++){
        bool useSmallFont = i==1 || i==5;
        touchMenuButtons[i].setText(text[i],  useSmallFont ? &buttonFontSmall : &buttonFont );
    }
    
    takeScreenshot = false;
    
}


//--------------------------------------------------------------
void Sequencer::hitBeat(void){
    onPreHit = false;
    thisBeat++;
    if (thisBeat >= NUM_BEATS){
        thisBeat = 0;
    }
    
    //cout<<"hit beat "<<thisBeat<<" at "<<ofGetElapsedTimef()<<endl;
    
    //play anything that's on
    bool playedSound = false;
    for (int k=0; k<NUM_SOUNDS; k++){
        if (beatsOn[thisBeat][k]){
            makeNewHit(k);
            playedSound = true;
        }
    }
    
    beatMarkers[thisBeat].triggerBeat();
    
    if (autoPlay && ofRandomuf() <  0.2){
        makeNewHit(ofRandom(15));
    }
    
    if (useClickTrack && !playedSound){
        if (thisBeat % 4 == 0){
            clickTrackSound2.trigger();
        }else{
            clickTrackSound.trigger();
        }
    }
}

//--------------------------------------------------------------
void Sequencer::update(){
    
    deltaTime = ofGetElapsedTimef() - prevFrameTime;
    prevFrameTime = ofGetElapsedTimef();
    
    if (!aboutScreen.isActive && hasAddedANote){
        firstRunTimer -= deltaTime;
    }
    
    for (int i=hits.size()-1; i>=0; i--){
        hits[i]->update(deltaTime);
        if (hits[i]->killMe){
            hits[i]->cleanUp();
            delete hits[i];
            hits.erase(hits.begin()+i);
        }
    }
    
    aboutScreen.update(deltaTime);
    stepModeInstructions.update(deltaTime);
    
    logo.update(deltaTime);
    
    //update the markers
    for (int i=0; i<NUM_BEATS; i++){
        beatMarkers[i].update(deltaTime, stepMode, firstRunTimer);
    }
    
    //update touch buttons
    for (int i=0; i<NUM_TOUCH_BUTTONS; i++){
        touchButtons[i].update(deltaTime);
    }
    for (int i=0; i<NUM_TOUCH_MENU_BUTTONS; i++){
        //TRYING OUT JUST ALWAYS HIDING THE LIVE/RECORD OPTION
//        //slide them to hide "Live/Record" during step mode
//        float targetY = menuButtonH * i;
//        if (stepMode)   targetY = menuButtonH * (i-1);
//        
//        if (i > 3){    //both tempo buttons should be on the same Y plane and click track needs to move up too
//            targetY -= menuButtonH;
//        }
//        
//        
//        float xeno = 0.85;
//        touchMenuButtons[i].box.y = xeno * touchMenuButtons[i].box.y + (1-xeno) * targetY;
        
        //actually update them
        touchMenuButtons[i].update(deltaTime);
    }
    aboutButton.update(deltaTime);
    for (int i=0; i<NUM_BEATS; i++){
        touchStepButtons[i].update(deltaTime);
    }
    
    for (int i=0; i<NUM_SOUNDS; i++){
        //check if this sound is used at all
        bool anyUse = false;
        for (int b=0; b< NUM_BEATS; b++){
            if (beatsOn[b][i]){
                anyUse = true;
                break;
            }
        }
        soundButtons[i].update(deltaTime, curStepSound, anyUse);
    }
    
    //if this is the first timer we're running, we want to hide the menu buttons at first
    if (firstRunTimer > 0){
        float slideInTime = 0.7;
        float prc = 1;
        prc = powf(prc, 2);
        if (firstRunTimer < slideInTime){
            prc = firstRunTimer/slideInTime;
        }
        
        float startMenuX = ofGetWidth() + 10;
        
        for (int i=0; i<NUM_TOUCH_MENU_BUTTONS; i++){
            float       targetMenuX = ofGetWidth() - touchMenuButtons[0].box.width;
            if (i==4)   targetMenuX =   ofGetWidth() - touchMenuButtons[0].box.width/2;
            
            touchMenuButtons[i].box.x = prc * startMenuX + (1.0f-prc) * targetMenuX;
        }
        
        aboutButton.box.x = prc * -aboutButton.box.width;
    }
    //otherwise just have them in the right place
    else{
        for (int i=0; i<NUM_TOUCH_MENU_BUTTONS; i++){
            float       menuX = ofGetWidth() - touchMenuButtons[0].box.width;
            if (i==4)   menuX = ofGetWidth() - touchMenuButtons[0].box.width/2;
            
            touchMenuButtons[i].box.x = menuX;
        }
        aboutButton.box.x = 0;
    }
    
    
    //testing
//    for (int i=0; i<NUM_IOS_BEATS_PER_SOUND; i++){
//        cout<<i<<"  "<<sounds[i][14].getPosition()<<endl;
//    }
}

//--------------------------------------------------------------
void Sequencer::draw(){
    
    
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
        bool isOn = false;
        bool altStepSoundIsOn = false;
        if (!stepMode){
            for (int k=0; k<NUM_SOUNDS; k++){
                if (beatsOn[i][k])  isOn = true;
            }
        }else{
            isOn = beatsOn[i][curStepSound];
            for (int k=0; k<NUM_SOUNDS; k++){
                if (beatsOn[i][k] && k != curStepSound){
                    altStepSoundIsOn = true;
                }
            }
        }
        
        beatMarkers[i].draw(isOn, altStepSoundIsOn, recording);
    }
    
    shader.end();
    
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
            
            
            int textFadeAlpa = 90;
            
            //draw the text for Live/Record
            if (i == MENU_BUTTON_LIVE){
                ofSetColor(0);
                
                float textCenterX = touchMenuButtons[i].box.x + touchMenuButtons[i].box.width*0.6;
                float textY = touchMenuButtons[i].box.y + touchMenuButtons[i].box.height/2 + buttonFontSmall.getLineHeight()/2;
                float textSpacing = touchMenuButtons[i].box.width * 0.07;
                
                float slashX = textCenterX - buttonFontSmall.stringWidth("/")/2;
                buttonFontSmall.drawString("/", slashX, textY);
                
                ofSetColor(0, recording ? 255 : textFadeAlpa);
                float normalX = textCenterX - buttonFontSmall.stringWidth("Record") - textSpacing;
                buttonFontSmall.drawString("Record", normalX, textY);
                
                ofSetColor(0, recording ?  textFadeAlpa : 255);
                float stepX = textCenterX + textSpacing*0.75;
                buttonFontSmall.drawString("Live", stepX, textY);
            }
            
            //draw the text for step/normal
            if (i == MENU_BUTTON_STEP){
                ofSetColor(0);
                
                float textCenterX = touchMenuButtons[i].box.x + touchMenuButtons[i].box.width*0.6;
                float textY = touchMenuButtons[i].box.y + touchMenuButtons[i].box.height/2 + buttonFontSmall.getLineHeight()/2;
                float textSpacing = touchMenuButtons[i].box.width * 0.07;
                
                float slashX = textCenterX - buttonFontSmall.stringWidth("/")/2;
                buttonFontSmall.drawString("/", slashX, textY);
                
                ofSetColor(0, stepMode ? textFadeAlpa : 255);
                float normalX = textCenterX - buttonFontSmall.stringWidth("Normal") - textSpacing;
                buttonFontSmall.drawString("Normal", normalX, textY);
                
                ofSetColor(0, stepMode ? 255 : textFadeAlpa);
                float stepX = textCenterX + textSpacing*0.6;
                buttonFontSmall.drawString("Step", stepX, textY);
            }
            
            //draw the text for click track
            if (i == MENU_BUTTON_CLICK){
                ofSetColor(0);
                
                float textCenterX = touchMenuButtons[i].box.x + touchMenuButtons[i].box.width*0.67;
                float textY = touchMenuButtons[i].box.y + touchMenuButtons[i].box.height/2 + buttonFontSmall.getLineHeight()/2;
                float textSpacing = touchMenuButtons[i].box.width * 0.05;
                
                float clickTextXPos = textCenterX -  buttonFontSmall.stringWidth("Click:On/");
                buttonFontSmall.drawString("Click:", clickTextXPos, textY);
                
                float slashX = textCenterX - buttonFontSmall.stringWidth("/")/2;
                buttonFontSmall.drawString("/", slashX, textY);
                
                ofSetColor(0, !useClickTrack ? textFadeAlpa : 255);
                float normalX = textCenterX - buttonFontSmall.stringWidth("On") - textSpacing;
                buttonFontSmall.drawString("On", normalX, textY);
                
                ofSetColor(0, !useClickTrack ? 255 : textFadeAlpa);
                float stepX = textCenterX + textSpacing*0.6;
                buttonFontSmall.drawString("Off", stepX, textY);
            }
        }
        
        //write the bpm
        ofSetColor(0);
        string bpmText = ofToString( (int)bpmValue);
        
        float bpmTextX = touchMenuButtons[MENU_BUTTON_TEMPO_UP].box.x+touchMenuButtons[MENU_BUTTON_TEMPO_UP].box.width - buttonFontSmall.stringWidth(bpmText)/2;
        float bpmTextY = touchMenuButtons[MENU_BUTTON_TEMPO_UP].box.y+touchMenuButtons[MENU_BUTTON_TEMPO_UP].box.height-10;
        buttonFontSmall.drawString(bpmText, bpmTextX, bpmTextY);
        
        //help button
        ofFill();
        ofSetColor(whiteVal);
        ofDrawRectangle(aboutButton.box);
        aboutButton.draw();
        ofSetColor(0);
        aboutButtonIcon.draw(aboutButton.box.x, aboutButton.box.y+aboutButton.box.getHeight()/2-aboutButton.box.getWidth()/2, aboutButton.box.getWidth(), aboutButton.box.getWidth());
        
        
        
        for (int i=0; i<NUM_SOUNDS; i++){
            soundButtons[i].draw();
        }
        ofDisableAlphaBlending();
    }
    
    
    
    //about screen info
    aboutScreen.draw();
    stepModeInstructions.draw();
    
    //the logo
    logo.draw();
    
//    bool showWIPText = !publicRelease;
//#ifdef USING_IOS
//    showWIPText = false;
//#endif
//    if (showWIPText){
//        ofSetColor(0);
//        string text =   "Sequencer by Andy Wallace - andy@andymakes.com";
//        text +=         "\nWork in progress. Please do not distribute.";
//        text +=         "\nPress h for help.";
//        ofDrawBitmapString(text, 10, 15);
//    }
    
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
    if (stepModeInstructions.isActive){
        stepModeInstructions.turnOff();
        return;
    }
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
        if (logo.timer < logo.growTime+ logo.pauseTime){
            logo.timer = logo.growTime+ logo.pauseTime;
        }
        aboutScreen.turnOn();
    }
    
    if (key == 's'){
        setStepMode(!stepMode);
    }
//    if (key == 'a'){
//        autoPlay = !autoPlay;
//    }
    if (key == 'f'){
        ofToggleFullscreen();
    }
    if (key == 'c'){
        turnOnRecordingWhenClearing = !turnOnRecordingWhenClearing;
    }
//    if (key == 'p'){
//        usePreHitDetection = !usePreHitDetection;
//    }
    if (key == '\\'){
        useNumpadKeys = !useNumpadKeys;
    }
    if (key == 'v'){
        setVisualEffect();
    }
    
    if (key == ' '){
        if (!stepMode){
            setRecording(!recording);
        }
    }
    
    if (key == 'p'){
        takeScreenshot = true;
    }
    
    
    if (key == ']'){
        skipIntro();
    }
    
    if (key == OF_KEY_UP){
        bpmValue += 10;
        bpmValue = MIN(bpmValue, 500);
        //bpm.setBpm(bpmValue);
    }
    if (key == OF_KEY_DOWN){
        bpmValue -= 10;
        bpmValue = MAX(bpmValue, 50);
        //bpm.setBpm(bpmValue);
    }
    if (key == 'b'){
        bpmValue = bpmStartValue;
        //bpm.setBpm(bpmValue);
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
    if (stepModeInstructions.isActive){
        stepModeInstructions.turnOff();
        return;
    }
    if (aboutScreen.isActive){
        aboutScreen.turnOff();
        return;
    }
    
    //help button overrides anything else
    if (aboutButton.checkHit(x, y) && !aboutScreen.isActive){
        if (logo.timer < logo.growTime+ logo.pauseTime){
            logo.timer = logo.growTime+ logo.pauseTime;
        }
        aboutScreen.turnOn();
        return;
    }
    
    //check the buttons!
    if (!stepMode){
        //beat markers
        for (int i=0; i<NUM_BEATS; i++){
            if(beatMarkers[i].checkHit(x, y)){
                for (int k=0; k<NUM_SOUNDS; k++){
                    beatsOn[i][k] = false;
                }
                beatMarkers[i].triggerClear();
                return;
            }
        }
        //sound buttons
        for (int i=0; i<NUM_TOUCH_BUTTONS; i++){
            if(touchButtons[i].checkHit(x, y)){
                makeNewHit(i);
                //if they switch to step mode, have this be the current sound
                curStepSound = i;
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
                playSound(i);
            }
        }
    }
    
    //menu buttons
    if (firstRunTimer <= 0){
        for (int i=0; i<NUM_TOUCH_MENU_BUTTONS; i++){
            if (touchMenuButtons[i].checkHit(x, y)){
                
                if (i == MENU_BUTTON_LIVE){
                    setRecording(!recording);
                }
                
                if (i == MENU_BUTTON_STEP){
                    setStepMode(!stepMode);
                }
                
                if (i == MENU_BUTTON_CLEAR){
                    clearBeats();
                }
                
                if (i == MENU_BUTTON_TEMPO_DOWN){
                    bpmValue -= 10;
                    bpmValue = MAX(bpmValue, 50);
                    //bpm.setBpm(bpmValue);
                }
                if (i == MENU_BUTTON_TEMPO_UP){
                    bpmValue += 10;
                    bpmValue = MIN(bpmValue, 500);
                    //bpm.setBpm(bpmValue);
                }
                
                if (i == MENU_BUTTON_CLICK){
                    useClickTrack = !useClickTrack;
                }
            }
        }
    }
    
}

//--------------------------------------------------------------
//This is only used for PC/Mac and is not required for functionality
void Sequencer::mouseMoved(int x, int y){
//    curMouseOverSound = -1;
//    for (int i=0; i<NUM_TOUCH_BUTTONS; i++){
//        if(touchButtons[i].checkHit(x, y, false)){
//            curMouseOverSound = i;
//        }
//    }
}


//--------------------------------------------------------------
void Sequencer::windowResized(int w, int h){
    gameW = ofGetWidth();
    gameH = ofGetHeight();
    
    aboutScreen.setup(whiteVal, usingIPad, false);
    if (!hasRunStepMode){
        stepModeInstructions.setup(whiteVal, usingIPad, true);
    }
    
    setButtonPositions();
    logo.setPos();
}

//--------------------------------------------------------------
void Sequencer::setButtonPositions(){
    
    float beatXPadding = beatXSpacing/2 + (ofGetWidth()-(beatXSpacing*NUM_BEATS))/2;
    float stepModeBeatSpacing = ofGetWidth()/NUM_BEATS;
    for (int i=0; i<NUM_BEATS; i++){
        float normX = beatXPadding+beatXSpacing*i;
        float stepX = stepModeBeatSpacing/2 + stepModeBeatSpacing*i;
        beatMarkers[i].setup(normX, stepX, ofGetHeight()-beatYDistFromBottom, whiteVal, usingIPad);
    }
    
    int buttonW = ofGetWidth()/8;
    int buttonH = ofGetHeight()/2;
    for (int i=0; i<NUM_TOUCH_BUTTONS; i++){
        //doing one less horizontal slot than we have to make room for the menu buttons
        int col = i%7;
        int row = i/7;

        //the last button that would get pushed off screen instead takes slot 7 on the bottom
        if (row == 2){
            col = 7;
            row = 1;
        }
        
        touchButtons[i].setup(col * buttonW, row * buttonH, buttonW, buttonH);
    }
    
    //the menu buttons go in the top right
    menuButtonH = buttonH / 5;
    for (int i=0; i<NUM_TOUCH_MENU_BUTTONS; i++){
        float startY = -menuButtonH;    //if you ever full on remove live/record, you can rmeove this too. This is just being used to hide that button
        string oldText = touchMenuButtons[i].text;
        ofTrueTypeFont * oldFont = touchMenuButtons[i].font;
        if (i != MENU_BUTTON_TEMPO_DOWN && i != MENU_BUTTON_TEMPO_UP){
            float thisY =  startY + i * menuButtonH;
            touchMenuButtons[i].setup(ofGetWidth() - buttonW, thisY, buttonW, menuButtonH);
        }else{
            float tempoX = i==MENU_BUTTON_TEMPO_DOWN ? ofGetWidth() - buttonW : ofGetWidth() - buttonW/2;
            touchMenuButtons[i].setup(tempoX, startY + MENU_BUTTON_TEMPO_DOWN * menuButtonH, buttonW/2, menuButtonH);
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
    aboutButton.setText("", &buttonFont);
    
    
    //sound buttons for step mode
    float soundSpacing = ((ofGetWidth()-buttonW) / 8) - 2;
    for (int i=0; i<NUM_SOUNDS; i++){
        float soundX = 5+soundSpacing*(i%8);
        if (i >= 8)  soundX += soundSpacing/2;
        float soundY = i < 8 ? 15 : 15+soundSpacing;
        soundButtons[i].setPosition(soundX, soundY, soundSpacing, soundSpacing);
    }
    
    //the step mode buttons
    int stepButtonW = ofGetWidth() / NUM_BEATS;
    for (int i=0; i<NUM_BEATS; i++){
        int yPos = soundSpacing * 2 + 30;   //give it a bit of extra spacing
        if (i>=NUM_BEATS-2){
            yPos = ofGetHeight()/2 - menuButtonH;
        }
        touchStepButtons[i].setup(i*stepButtonW, yPos, stepButtonW, ofGetHeight());
    }
}

//--------------------------------------------------------------
void Sequencer::makeNewHit(int idNum){
    //assume we'll play the sound
    bool bPlaySound = true;
    
    //turning this beat on
    if (recording){
        int beatPos = thisBeat;
        if (onPreHit){
            bPlaySound = false;  //we'll catch it in a second
            beatPos = (thisBeat+1)%NUM_BEATS;
        }
        beatsOn[beatPos][idNum] = true;
    }
    
    //Making the hit
    if (bPlaySound){
        Hit * thisHit;
        
        if (idNum == 0)     thisHit = new DotPolygonHit();
        if (idNum == 1)     thisHit = new SweepHit();
        if (idNum == 2)     thisHit = new BuckshotHit();
        if (idNum == 3)     thisHit = new ClapHit();
        if (idNum == 4)     thisHit = new CrystalHit();
        if (idNum == 5)     thisHit = new TriangleHit();
        if (idNum == 6)     thisHit = new DrunkTriangleHit();
        if (idNum == 7)     thisHit = new TunnelHit();
        if (idNum == 8)     thisHit = new SizzleHit();
        if (idNum == 9)     thisHit = new ChaserHit();
        if (idNum == 10)     thisHit = new SquareHit();
        if (idNum == 11)     thisHit = new WaveColumnHit();
        if (idNum == 12)     thisHit = new GrapesHit();
        if (idNum == 13)     thisHit = new SlashHit();
        if (idNum == 14)     thisHit = new TrapezoidHit();
        
        thisHit->setup(gameW, gameH, whiteVal, usingIPad);
        
        hits.push_back(thisHit);
    }
    
    //making the sound
    if (bPlaySound){
        playSound(idNum);
    }
    
    hasAddedANote = true;
    
}

//--------------------------------------------------------------
void Sequencer::playSound(int idNum){
    sounds[ thisBeat%NUM_IOS_BEATS_PER_SOUND ][idNum].trigger();
//#ifdef USING_IOS
//    sounds[ thisBeat%NUM_IOS_BEATS_PER_SOUND ][idNum].trigger();
//    //sounds[ 0 ][idNum].trigger();
//#else
//    //sounds[0][idNum].trigger();
//#endif
//#ifdef USING_IOS
//    sounds[ thisBeat%NUM_IOS_BEATS_PER_SOUND ][idNum].play();
//#else
//    sounds[0][idNum].play();
//#endif
}

//--------------------------------------------------------------
void Sequencer::setStepMode(bool isOn){
    stepMode = isOn;
    for (int i=0; i<NUM_SOUNDS; i++){
        if(stepMode){
            soundButtons[i].stepModeOn();
            setRecording(true);
            //is this the first time running step mode?
            if (!hasRunStepMode){
                cout<<"open the step mode instructions"<<endl;
                if (logo.timer < logo.growTime+ logo.pauseTime){
                    logo.timer = logo.growTime+ logo.pauseTime;
                }
                stepModeInstructions.turnOn();
                saveHasRunStepMode();
            }
        }else{
            soundButtons[i].stepModeOff();
        }
    }
    
}

//--------------------------------------------------------------
void Sequencer::stepModePress(int placeID){
    beatsOn[placeID][curStepSound] = !beatsOn[placeID][curStepSound];
}

//--------------------------------------------------------------
void Sequencer::setRecording(bool isOn){
    recording = isOn;
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
        
        //first two files are click track
        clickTrackSound.load(ofToDataPath(files[0]));
        clickTrackSound.setPosition(1);
        clickTrackSound2.load(ofToDataPath(files[1]));
        clickTrackSound2.setPosition(1);
        
        cout<<"found "<<files.size()<<" sound files"<<endl;
        
        for (int i=0; i<NUM_SOUNDS; i++){
//#ifdef USING_IOS
            for (int k=0; k<NUM_IOS_BEATS_PER_SOUND; k++){
                sounds[k][i].load(ofToDataPath(files[i+2]));
                sounds[k][i].setPosition(1);    //put it at the end
            }
//#else
            //sounds[0][i].load(ofToDataPath(files[i+2]));    //TESTING
            //sounds[0][i].setMultiPlay(true);
//#endif
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


//--------------------------------------------------------------
bool Sequencer::checkIsFirstRun(){
    string fileName = "hasrun.txt";
    
    //getting the path on mac
    string appPath = ofFilePath::getCurrentExeDir();
    appPath+="../Resources/";
    
#ifdef USING_IOS
    appPath = iosDataPath;
#endif
    
    //cout<<appPath<<endl;
    
    //we're going to check if a specific file exists
    ofFile checkFile(appPath+fileName);
    if (checkFile.exists()){
        return false;
    }
    
    else{
        cout<<"First run!"<<endl;
        //go ahead and make the file
        ofstream newFile;
        #ifdef USING_IOS
        newFile.open(appPath+fileName);
        #else
        newFile.open(fileName);
        #endif
        
        if (newFile.is_open()){
            newFile<<"you ran it";
        }else{
            cout<<"something bad hapenned"<<endl;
        }
        newFile.close();
        
        return  true;
    }

}

//--------------------------------------------------------------
bool Sequencer::checkHasRunStepMode(){
    string fileName = "hasdonestep.txt";
    //getting the path on mac
    string appPath = ofFilePath::getCurrentExeDir();
    appPath+="../Resources/";
    
#ifdef USING_IOS
    appPath = iosDataPath;
#endif
    
    //we're going to check if a specific file exists
    ofFile checkFile(appPath+fileName);
    if (checkFile.exists()){
        return true;
    }
    
    return false;
}

//--------------------------------------------------------------
void Sequencer::saveHasRunStepMode(){
    cout<<"save step mode"<<endl;
    
    hasRunStepMode = true;
    
    string fileName = "hasdonestep.txt";
    //getting the path on mac
    string appPath = ofFilePath::getCurrentExeDir();
    appPath+="../Resources/";
    
#ifdef USING_IOS
    appPath = iosDataPath;
#endif
    
    //go ahead and make the file
    ofstream newFile;
#ifdef USING_IOS
    newFile.open(appPath+fileName);
#else
    newFile.open(fileName);
#endif
    
    if (newFile.is_open()){
        newFile<<"you ran it";
    }else{
        cout<<"something bad hapenned"<<endl;
    }
    newFile.close();
}

//--------------------------------------------------------------
void Sequencer::skipIntro(){
    cout<<"kill it"<<endl;
    firstRunTimer = 0;
    hasAddedANote = true;
}



//--------------------------------------------------------------
void Sequencer::setMaximAudio(bool advanceAudioThisCycle){
    /* Stick your maximilian 'play()' code in here ! Declare your objects in testApp.h.
     For information on how maximilian works, take a look at the example code at
     http://www.maximilian.strangeloop.co.uk
     under 'Tutorials'.
     */
    
    double counterVal = timer.phasor(bpmValue/60.0f);   //this sets up a metronome
    int currentCount=(int)counterVal;                   //currentCount will become 1 at the last tick of eahc cycle
    
    //checking for pre-hits
    if (counterVal > preHitPrc && !onPreHit && usePreHitDetection){
        //cout<<"pre hit at "<<ofGetElapsedTimef()<<endl;
        onPreHit = true;
    }
    
    //checking for full on beats!
    if (currentCount != 0) {
        hitBeat();
    }
    
    //really do not know why, but I need to advance the audio every other cycle otherwise it plays back at double speed
    if (advanceAudioThisCycle){
        sampleOut = 0;
        
        sampleOut+=clickTrackSound.playOnce();
        sampleOut+=clickTrackSound2.playOnce();
        
        for (int i=0; i<NUM_SOUNDS; i++){
            for (int k=0; k<NUM_IOS_BEATS_PER_SOUND; k++){
                sampleOut += sounds[k][i].playOnce();
            }
        }
    }
    
}






