#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetOrientation(OF_ORIENTATION_90_LEFT);
    
    sequencer.iosDataPath = ofxiOSGetDocumentsDirectory();
    sequencer.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    sequencer.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    sequencer.draw();
	
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
    sequencer.touchDown(touch.x, touch.y);
    
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){
    //cout<<"touch "<<touch.x<<" "<<touch.y<<endl;
    if (touch.x < 100 && touch.y < 100){
        sequencer.skipIntro();
        sequencer.clearBeats();
    }
}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::lostFocus(){

}

//--------------------------------------------------------------
void ofApp::gotFocus(){

}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){
    cout<<"new orientation: "<<newOrientation<<endl;
    
    if (newOrientation == OF_ORIENTATION_90_RIGHT || newOrientation == OF_ORIENTATION_90_LEFT){
        ofSetOrientation( (ofOrientation)newOrientation);
    }
}

