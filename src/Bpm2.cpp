//
//  Bpm2.cpp
//  bleepspace
//
//  Created by Andrew Wallace on 6/5/17.
//
//

#include "Bpm2.hpp"

void Bpm2::start(float bpmValue){
    setPreHitPrcSpacing(0.15);
    setBpm(bpmValue);
    
    timer.setPeriodicEvent(millisBetweenBeats*1000000); //converting to nanos
    startThread();
}

void Bpm2::stop(){
    stopThread();
}

void Bpm2::setPreHitPrcSpacing(float prc){
    preHitPrc = prc;
    preHitMilliRange = millisBetweenBeats * preHitPrc;
}

void Bpm2::setBpm(float newBpm){
    millisBetweenBeats = 60000 / newBpm;
    //nextBeatTime = ofGetElapsedTimeMillis() - (ofGetElapsedTimeMillis()%millisBetweenBeats) + millisBetweenBeats;
    //preHitMilliRange = millisBetweenBeats * preHitPrc;
}

void Bpm2::threadedFunction(){
    while(isThreadRunning()){
        timer.waitNext();
        cout<<"love 2 kill "<<ofGetElapsedTimeMillis()<<endl;
        ofNotifyEvent(beatEvent);
    }
}
