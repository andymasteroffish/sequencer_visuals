#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(240);
    
    fft.setup(NUM_BANDS);
    showFFT = false;
    autoPlay = false;

    prevFrameTime = ofGetElapsedTimef();
    deltaTime = 0;
    
    timeSinceLastHit = 0;
    minTimeForNextHit = 0.03;
}

//--------------------------------------------------------------
void ofApp::update(){
    deltaTime = ofGetElapsedTimef() - prevFrameTime;
    prevFrameTime = ofGetElapsedTimef();
    
    timeSinceLastHit += deltaTime;
    
    fft.update();
    
    //check if the FFT has a new hit for is
    if (fft.haveNewHit){
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
        makeNewTestHit(ofRandom(1,10));
    }
    
    //cout<<"hits: "<<hits.size()<<endl;
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
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'h'){
        showFFT = !showFFT;
    }
    if (key == 'a'){
        autoPlay = !autoPlay;
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
    idNum = idNum % 8;
    
    Hit * thisHit;
    
    if (idNum == 0)     thisHit = new TunnelHit();
    if (idNum == 1)     thisHit = new SweepHit();
    if (idNum == 2)     thisHit = new TriangleHit();
    if (idNum == 3)     thisHit = new GrapesHit();
    if (idNum == 4)     thisHit = new BuckshotHit();
    if (idNum == 5)     thisHit = new ChaserHit();
    if (idNum == 6)     thisHit = new SlashHit();
    if (idNum == 7)     thisHit = new SquareHit();
    
    thisHit->setup(bandsOnMicro, ofGetWidth(), ofGetHeight());
    
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
    
    thisHit->setup(bandsOnMicro, ofGetWidth(), ofGetHeight());
    
    hits.push_back(thisHit);
    
}
