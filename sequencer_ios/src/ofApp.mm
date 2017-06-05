#include "ofApp.h"


#import <AVFoundation/AVBase.h>
#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSDate.h>	/* for NSTimeInterval */
#import <AvailabilityMacros.h>
#import <CoreAudio/CoreAudioTypes.h>

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
    
    
    //for maxim
    ofSoundStreamSetup(2,0,this, sequencer.sampleRate, sequencer.bufferSize, 4);
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
        
        /* Stick your maximilian 'play()' code in here ! Declare your objects in testApp.h.
         
         For information on how maximilian works, take a look at the example code at
         
         http://www.maximilian.strangeloop.co.uk
         
         under 'Tutorials'.
         
         */
        
//        sequencer.currentCount=(int)sequencer.timer.phasor(8);//this sets up a metronome that ticks 8 times a second
//        
//        
//        if (sequencer.lastCount!=sequencer.currentCount) {//if we have a new timer int this sample, play the sound
//            
//            sequencer.kicktrigger=sequencer.hit[sequencer.playHead%16];//get the value out of the array for the kick
//            sequencer.snaretrigger=sequencer.snarehit[sequencer.playHead%16];//same for the snare
//            sequencer.playHead++;//iterate the playhead
//            sequencer.lastCount=0;//reset the metrotest
//            
//            cout<<"count "<<sequencer.playHead<<" at "<<ofGetElapsedTimef()<<endl;
//        }
//        
//        if (sequencer.kicktrigger==1) {//if the sequence has a 1 in it
//            
//            sequencer.kick.trigger();//reset the playback position of the sample to 0 (the beginning)
//            
//        }
//        
//        if (sequencer.snaretrigger==1) {
//            
//            sequencer.snare.trigger();//likewise for the snare
//            
//        }
//        
//        sequencer.sampleOut=sequencer.kick.playOnce()+sequencer.snare.playOnce();//just play the file. No looping.
//        
//        output[i*nChannels    ] = sequencer.sampleOut;
//        output[i*nChannels + 1] = sequencer.sampleOut;
//        
//        //cout<<"sample "<<sampleOut<<endl;
//        
//        sequencer.kicktrigger = 0;//set trigger to 0 at the end of each sample to guarantee retriggering.
//        sequencer.snaretrigger = 0;
        
    }
    
    
}

