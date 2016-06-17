#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    
    whiteVal = 240;
    ofBackground(whiteVal);
    
    bpm.reset();
    bpm.start();
    bpm.setBpm(180);
    bpm.setBeatPerBar(8);
    ofAddListener(bpm.beatEvent, this, &ofApp::hitBeat);
    thisBeat = 0;
    
    fft.setup(NUM_BANDS);
    showFFT = false;
    autoPlay = false;
    
    disableMic = true;

    prevFrameTime = ofGetElapsedTimef();
    deltaTime = 0;
    
    timeSinceLastHit = 0;
    minTimeForNextHit = 0.03;
    
    //beat timing
    
//    beatSpacing = 0.1;
//    beatTimer = 0;
    
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
    string soundFolder = "/Users/awallace/Documents/projects/sequencer/sounds/high_depth/";
    sounds[0].load(soundFolder+"agogo.wav");
    sounds[1].load(soundFolder+"cl_hat.wav");
    sounds[2].load(soundFolder+"clap.wav");
    sounds[3].load(soundFolder+"claves.wav");
    sounds[4].load(soundFolder+"crash.wav");
    sounds[5].load(soundFolder+"high_tom.wav");
    sounds[6].load(soundFolder+"kick.wav");
    sounds[7].load(soundFolder+"lo_tom.wav");
    sounds[8].load(soundFolder+"op_hat.wav");
    sounds[9].load(soundFolder+"snare.wav");
    for (int i=0; i<NUM_SOUNDS; i++){
        sounds[i].setMultiPlay(true);
    }
    
    //set the markers
    beatXSpacing = 40;
    beatYDistFromBottom = 40;
    float beatXPadding = (ofGetWidth()-(beatXSpacing*NUM_BEATS))/2;
    for (int i=0; i<NUM_BEATS; i++){
        beatMarkers[i].setup(beatXPadding+beatXSpacing*i, ofGetHeight()-beatYDistFromBottom);
    }
    
    
    clearBeats();
    
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
            makeNewTestHit(k);
        }
    }
    
    beatMarkers[thisBeat].triggerBeat();
    
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
    
    
    if (autoPlay && ofGetFrameNum() % 10 == 0){
        makeNewTestHit(ofRandom(1,16));
    }
    
    //update the beats
//    beatTimer += deltaTime;
//    if (beatTimer >= beatSpacing){
//        beatTimer = 0;
//        thisBeat++;
//        if (thisBeat >= NUM_BEATS){
//            thisBeat = 0;
//        }
//        
//        //play anything that's on
//        for (int k=0; k<NUM_SOUNDS; k++){
//            if (beatsOn[thisBeat][k]){
//                makeNewTestHit(k);
//            }
//        }
//        
//        beatMarkers[thisBeat].triggerBeat();
//    }
    
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
    
//    ofSetColor(0);
//    ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 15);
    
    
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
    if (key == 'c'){
        clearBeats();
    }
    
    if (key == '1')     makeNewTestHit(1);
    if (key == '2')     makeNewTestHit(2);
    if (key == '3')     makeNewTestHit(3);
    if (key == '4')     makeNewTestHit(4);
    if (key == '5')     makeNewTestHit(5);
    if (key == '6')     makeNewTestHit(6);
    if (key == '7')     makeNewTestHit(7);
    if (key == '8')     makeNewTestHit(8);
    if (key == '9')     makeNewTestHit(9);
    if (key == '0')     makeNewTestHit(10);
    if (key == 'q')     makeNewTestHit(11);
    if (key == 'w')     makeNewTestHit(12);
    if (key == 'e')     makeNewTestHit(13);
    if (key == 'r')     makeNewTestHit(14);
    if (key == 't')     makeNewTestHit(15);
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

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
void ofApp::makeNewTestHit(int idNum){
    
    bool bandsOnMicro[NUM_BANDS/2];
    for (int i=0; i<NUM_BANDS/2; i++){
        bandsOnMicro[i] = ofRandomuf() > 0.5;
    }
    
    Hit * thisHit;
    
    if (idNum == 1)     thisHit = new TunnelHit();
    if (idNum == 2)     thisHit = new SweepHit();
    if (idNum == 3)     thisHit = new TriangleHit();
    if (idNum == 4)     thisHit = new GrapesHit();
    if (idNum == 5)     thisHit = new BuckshotHit();
    if (idNum == 6)     thisHit = new ChaserHit();
    if (idNum == 7)     thisHit = new SlashHit();
    if (idNum == 8)     thisHit = new SquareHit();
    if (idNum == 9)     thisHit = new TrapazoidHit();
    if (idNum == 10)     thisHit = new DotPolygonHit();
    if (idNum == 11)     thisHit = new SizzleHit();
    if (idNum == 12)     thisHit = new DrunkTriangleHit();
    if (idNum == 13)     thisHit = new CrossBoxHit();
    if (idNum == 14)     thisHit = new ClapHit();
    if (idNum == 15)     thisHit = new WaveColumnHit();
    
    thisHit->setup(bandsOnMicro, ofGetWidth(), ofGetHeight(), whiteVal);
    
    hits.push_back(thisHit);
    
    //testing
    if (idNum < NUM_SOUNDS){
        sounds[idNum].play();
        beatsOn[thisBeat][idNum] = true;
    }
//    if (idNum == NUM_SOUNDS){
//        sounds[0].play();
//        beatsOn[thisBeat][0] = true;
//    }
    
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
