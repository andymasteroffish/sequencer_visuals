#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    fft.setup(NUM_BANDS);
    showFFT = false;

    prevFrameTime = ofGetElapsedTimef();
    deltaTime = 0;
    
    timeSinceLastHit = 0;
    minTimeForNextHit = 0.02;
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
    
    if (key == '1')     makeNewTestHit(1);
    if (key == '2')     makeNewTestHit(2);
    if (key == '3')     makeNewTestHit(3);
    if (key == '4')     makeNewTestHit(4);
    if (key == '5')     makeNewTestHit(5);
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
//    cout<<"beat ";
//    for (int i=0; i<NUM_BANDS; i++){
//        cout<<bandsOn[i];
//    }
//    cout<<endl;
    Hit * thisHit = new TriangleHit();
    thisHit->setup(bandsOn, ofGetWidth(), ofGetHeight());
    
    hits.push_back(thisHit);
}

//--------------------------------------------------------------
void ofApp::makeNewTestHit(int idNum){
    
    bool bandsOn[NUM_BANDS];
    for (int i=0; i<NUM_BANDS; i++){
        bandsOn[i] = ofRandomuf() > 0.5;
    }
    
    Hit * thisHit;
    
    if (idNum == 1)     thisHit = new TunnelHit();
    if (idNum == 2)     thisHit = new SweepHit();
    if (idNum == 3)     thisHit = new TriangleHit();
    if (idNum == 4)     thisHit = new GrapesHit();
    if (idNum == 5)     thisHit = new BuckshotHit();
    
    thisHit->setup(bandsOn, ofGetWidth(), ofGetHeight());
    
    hits.push_back(thisHit);
    
}
