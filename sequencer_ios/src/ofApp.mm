#include "ofApp.h"


#import <AVFoundation/AVBase.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSDate.h>	/* for NSTimeInterval */
#import <AvailabilityMacros.h>
#import <CoreAudio/CoreAudioTypes.h>
#import<AVFoundation/AVFoundation.h>    //this import is needed to fuck with AVAudioSession

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetOrientation(OF_ORIENTATION_90_RIGHT);//LEFT);
    
    sequencer.iosDataPath = ofxiOSGetDocumentsDirectory();
    sequencer.setup();
    
    
    //TO MAKE THE AUDIO WORK, I CHANGED AVSoundPlayer.m
    //[[AVAudioSession sharedInstance] setCategory: AVAudioSessionCategoryPlayback error: nil];
    //became
    //[[AVAudioSession sharedInstance] setCategory: AVAudioSessionCategorySoloAmbient error: nil];
    //When it was set to AVAudioSessionCategoryPlayback audio would play even when the phone was muted
    
    //you'll also want to edit ofxiOSSoundStream now the you use maxim
    
    
    //for maxim
    ofSoundStreamSetup(2,0,this, sequencer.sampleRate, sequencer.bufferSize, 4);
    
    ofxiOSSoundStream::setMixWithOtherApps(true);
    
    //this needs to be set to allow bluetooth playback
    [[AVAudioSession sharedInstance] setCategory: AVAudioSessionCategoryPlayback error: nil];
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
    //sequencer.bpm.justGotFocus();
}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){
    //cout<<"new orientation: "<<newOrientation<<endl;
    
    if (newOrientation == OF_ORIENTATION_90_RIGHT || newOrientation == OF_ORIENTATION_90_LEFT){
        ofSetOrientation( (ofOrientation)newOrientation);
    }
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    
    
    for (int i = 0; i < bufferSize; i++){
        
        sequencer.setMaximAudio(i%2==0);
        output[i*nChannels    ] = sequencer.sampleOut;
        output[i*nChannels + 1] = sequencer.sampleOut;
        
    }
    
    
}

