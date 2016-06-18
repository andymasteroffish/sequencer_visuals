#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    
    whiteVal = 240;
    ofBackground(whiteVal);
    
    bpm.reset();
    bpmStartValue = 160;
    bpmValue = bpmStartValue;
    bpm.setBpm(bpmValue);
    bpm.setBeatPerBar(8);
    ofAddListener(bpm.beatEvent, this, &ofApp::hitBeat);
    bpm.start();
    
    thisBeat = 0;
    
    fft.setup(NUM_BANDS);
    showFFT = false;
    autoPlay = false;
    
    recording = false;
    
    disableMic = true;

    prevFrameTime = ofGetElapsedTimef();
    deltaTime = 0;
    
    timeSinceLastHit = 0;
    minTimeForNextHit = 0.03;
    
    
    //randomzie hit IDs on start
    for (int i=0; i<15; i++){
        hitIDs[i] = i+1;
    }
    for (int i=0; i<1000; i++){
        int a = (int)ofRandom(15);
        int b = (int)ofRandom(15);
        int temp = hitIDs[a];
        hitIDs[a] = hitIDs[b];
        hitIDs[b] = temp;
    }
    
    //set the sounds
    string soundFolder = "/Users/awallace/Documents/projects/sequencer/sounds/good_ones_edits/";
    
    sounds[0].load(soundFolder+"low_depth_0_deep_kick.wav");
    sounds[1].load(soundFolder+"clap.wav");
    sounds[2].load(soundFolder+"mid_time_8_long_clang.wav");
    sounds[3].load(soundFolder+"mid_time_9_foil.wav");
    sounds[4].load(soundFolder+"lo_tom.wav");
    sounds[5].load(soundFolder+"low_depth_2.wav");
    sounds[6].load(soundFolder+"randoms_0.wav");
    sounds[7].load(soundFolder+"low_depth_8_clang.wav");
    sounds[8].load(soundFolder+"randoms_4_clink.wav");
    sounds[9].load(soundFolder+"low_depth_6_clap.wav");
    sounds[10].load(soundFolder+"randoms_3_bend_laser.wav");
    sounds[11].load(soundFolder+"mid_time_3.wav");
    sounds[12].load(soundFolder+"low_depth_1.wav");
    sounds[13].load(soundFolder+"low_depth_5_sizzle.wav");
    sounds[14].load(soundFolder+"mid_time_4.wav");
    
    
    ofDirectory dir;
//    dir.listDir(soundFolder);
//    for (int i=0; i<NUM_SOUNDS; i++){
//        sounds[i].load(dir.getPath(9));
//        cout<<i<<" : "<<dir.getPath(9)<<endl;
//    }
    
//    sounds[0].load(soundFolder+"agogo.wav");
//    sounds[1].load(soundFolder+"cl_hat.wav");
//    sounds[2].load(soundFolder+"clap.wav");
//    sounds[3].load(soundFolder+"claves.wav");
//    sounds[4].load(soundFolder+"crash.wav");
//    sounds[5].load(soundFolder+"high_tom.wav");
//    sounds[6].load(soundFolder+"kick.wav");
//    sounds[7].load(soundFolder+"lo_tom.wav");
//    sounds[8].load(soundFolder+"op_hat.wav");
//    sounds[9].load(soundFolder+"snare.wav");
    
    
    for (int i=0; i<NUM_SOUNDS; i++){
        sounds[i].setMultiPlay(true);
    }
    
    clearBeats();
    
    //set the markers
    beatXSpacing = 40;
    beatYDistFromBottom = 40;
    float beatXPadding = (ofGetWidth()-(beatXSpacing*NUM_BEATS))/2;
    for (int i=0; i<NUM_BEATS; i++){
        beatMarkers[i].setup(beatXPadding+beatXSpacing*i, ofGetHeight()-beatYDistFromBottom);
    }
    
}

//--------------------------------------------------------------
void ofApp::hitBeat(void){
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
void ofApp::update(){
    deltaTime = ofGetElapsedTimef() - prevFrameTime;
    prevFrameTime = ofGetElapsedTimef();
    
    timeSinceLastHit += deltaTime;
    
    fft.update();
    
    //check if the FFT has a new hit for is
    if (fft.haveNewHit && !disableMic){
        makeNewHit(fft.bandsOn);
    }
        
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
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (showFFT){
        fft.draw();
        ofSetColor(0);
        string debugText = "";
        debugText += "active: "+ofToString(hits.size())+"\n";
        ofDrawBitmapString(debugText, 10, 10);
    }
    

    for (int i=0; i<hits.size(); i++){
        hits[i]->draw();
    }
    
    //draw the markers
    for (int i=0; i<NUM_BEATS; i++){
        bool anyOn = false;
        for (int k=0; k<NUM_SOUNDS; k++){
            if (beatsOn[i][k])  anyOn = true;
        }
        
        beatMarkers[i].draw(anyOn);
    }
    
    if (recording){
        ofSetColor(0);
        ofDrawBitmapString("recording", 10, 15);
    }
    
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'h'){
        showFFT = !showFFT;
    }
    if (key == 'a'){
        autoPlay = !autoPlay;
    }
    if (key == 'f'){
        ofToggleFullscreen();
    }
    if (key == 'c' || key == 127){  //backspace
        clearBeats();
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
    if (key == OF_KEY_LEFT){
        bpmValue = bpmStartValue;
        bpm.setBpm(bpmValue);
    }
    
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
    
}


//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    //set the markers
    float beatXPadding = (ofGetWidth()-(beatXSpacing*NUM_BEATS))/2;
    for (int i=0; i<NUM_BEATS; i++){
        beatMarkers[i].pos.set(beatXPadding+beatXSpacing*i, ofGetHeight()-beatYDistFromBottom);
    }
}

//--------------------------------------------------------------
void ofApp::makeNewHit(bool bandsOn[NUM_BANDS]){
    if (timeSinceLastHit < minTimeForNextHit){
        return;
    }
    
    timeSinceLastHit = 0;
    
    //set half bands
    bool bandsOnMicro[NUM_BANDS/2];
    for (int i=0; i<NUM_BANDS; i+=2){
        bandsOnMicro[i/2] = bandsOn[i] || bandsOn[i+1];
    }
    
    //get the total value
    int idNum = 0;
    for (int i=0; i<NUM_BANDS/2; i++){
        if(bandsOnMicro[i]){
            idNum += pow(2,i);
        }
    }
    
    //testing
    //idNum = idNum % 9;
    
    Hit * thisHit;
    
    if (idNum == 0)     thisHit = new TunnelHit();  //THIS CAN NEVER BE 0, BECAUSE AT LEAST ONE BAND MUST BE ON
    
    if (idNum == hitIDs[0])     thisHit = new TunnelHit();
    if (idNum == hitIDs[1])     thisHit = new SweepHit();
    if (idNum == hitIDs[2])     thisHit = new TriangleHit();
    if (idNum == hitIDs[3])     thisHit = new GrapesHit();
    if (idNum == hitIDs[4])     thisHit = new BuckshotHit();
    if (idNum == hitIDs[5])     thisHit = new ChaserHit();
    if (idNum == hitIDs[6])     thisHit = new SlashHit();
    if (idNum == hitIDs[7])     thisHit = new SquareHit();
    if (idNum == hitIDs[8])     thisHit = new TrapazoidHit();
    if (idNum == hitIDs[9])     thisHit = new DotPolygonHit();
    if (idNum == hitIDs[10])    thisHit = new SizzleHit();
    if (idNum == hitIDs[11])    thisHit = new DrunkTriangleHit();
    if (idNum == hitIDs[12])    thisHit = new CrossBoxHit();
    if (idNum == hitIDs[13])    thisHit = new ClapHit();
    if (idNum == hitIDs[14])    thisHit = new WaveColumnHit();
    
    thisHit->setup(bandsOnMicro, ofGetWidth(), ofGetHeight(), whiteVal);
    
    hits.push_back(thisHit);
}

//--------------------------------------------------------------
void ofApp::makeNewHit(int idNum){
    
    bool bandsOnMicro[NUM_BANDS/2];
    for (int i=0; i<NUM_BANDS/2; i++){
        bandsOnMicro[i] = ofRandomuf() > 0.5;
    }
    
    Hit * thisHit;
    
    if (idNum == 0)     thisHit = new TunnelHit();
    if (idNum == 1)     thisHit = new SweepHit();
    if (idNum == 2)     thisHit = new TriangleHit();
    if (idNum == 3)     thisHit = new GrapesHit();
    if (idNum == 4)     thisHit = new BuckshotHit();
    if (idNum == 5)     thisHit = new ChaserHit();
    if (idNum == 6)     thisHit = new SlashHit();
    if (idNum == 7)     thisHit = new SquareHit();
    if (idNum == 8)     thisHit = new TrapazoidHit();
    if (idNum == 9)     thisHit = new DotPolygonHit();
    if (idNum == 10)     thisHit = new SizzleHit();
    if (idNum == 11)     thisHit = new DrunkTriangleHit();
    if (idNum == 12)     thisHit = new CrossBoxHit();
    if (idNum == 13)     thisHit = new ClapHit();
    if (idNum == 14)     thisHit = new WaveColumnHit();
    
    thisHit->setup(bandsOnMicro, ofGetWidth(), ofGetHeight(), whiteVal);
    
    hits.push_back(thisHit);
    
    //turnign this beat on
    if (idNum < NUM_SOUNDS){
        sounds[idNum].play();
        if (recording){
            beatsOn[thisBeat][idNum] = true;
        }
    }
    
}



//--------------------------------------------------------------
void ofApp::clearBeats(){
    for (int i=0; i<NUM_BEATS; i++){
        for (int k=0; k<NUM_SOUNDS; k++){
            beatsOn[i][k] = false;
        }
        beatMarkers[i].triggerClear();
    }
}
