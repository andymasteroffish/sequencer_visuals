#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    sequencer.setup();
    
}


//--------------------------------------------------------------
void ofApp::update(){
    
    sequencer.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    sequencer.draw();
    
    if (sequencer.takeScreenshot){
        sequencer.takeScreenshot = false;
        ofImage pic;
        pic.grabScreen(0, 0 , ofGetWidth(), ofGetHeight());
        pic.save("screenshots/img"+ofToString(ofGetSystemTime())+".png");
    }
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    sequencer.touchDown(x, y);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    sequencer.keyPressed(key);
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    sequencer.mouseMoved(x, y);
}



//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
    sequencer.windowResized(w, h);
    
}

