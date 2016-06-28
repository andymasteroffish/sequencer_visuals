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
    
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    sequencer.keyPressed(key);
    
}


//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
    sequencer.windowResized(w, h);
    
}

