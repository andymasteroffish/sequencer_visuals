//
//  Bpm.cpp
//  sequncer
//
//  Created by Andrew Wallace on 7/5/16.
//
//

#include "Bpm.hpp"

void Bpm::start(float bpmValue){
    setPreHitPrcSpacing(0.15);
    setBpm(bpmValue);
    //cout<<"next beat time "<<nextBeatTime<<endl;
    startThread();
}

void Bpm::stop(){
    stopThread();
}

void Bpm::setPreHitPrcSpacing(float prc){
    preHitPrc = prc;
    preHitMilliRange = millisBetweenBeats * preHitPrc;
}

void Bpm::setBpm(float newBpm){
    millisBetweenBeats = 60000 / newBpm;
    nextBeatTime = ofGetElapsedTimeMillis() - (ofGetElapsedTimeMillis()%millisBetweenBeats) + millisBetweenBeats;
    preHitMilliRange = millisBetweenBeats * preHitPrc;
}

void Bpm::justGotFocus(){
    nextBeatTime = ofGetElapsedTimeMillis() + millisBetweenBeats;
}

void Bpm::threadedFunction(){
    
    while( isThreadRunning() ){
        
        if ( lock() ){
            
            //cout<<"thread "<<ofGetElapsedTimeMillis()<<endl;
            
            if (!hasDonePreHit && ofGetElapsedTimeMillis() >= nextBeatTime-preHitMilliRange){
                //cout<<"pre hit "<<ofGetElapsedTimeMillis()<<endl;
                ofNotifyEvent(preBeatEvent);
                hasDonePreHit = true;
            }
            else if (ofGetElapsedTimeMillis() >= nextBeatTime){
                //if (ofGetElapsedTimeMillis() != nextBeatTime){
                //    cout<<"beat on "<<nextBeatTime<<endl;
                //    cout<<"exact time "<<ofGetElapsedTimeMillis()<<endl;
                //}
                nextBeatTime += millisBetweenBeats;
                ofNotifyEvent(beatEvent);
                hasDonePreHit = false;
            }
            
            //we're done so we can unlock it
            unlock();
            //sleep(millisBetweenBeats);
            yield();
        }
        
    }
}
