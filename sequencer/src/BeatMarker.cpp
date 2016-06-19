//
//  BeatMarker.cpp
//  sequencer
//
//  Created by Andrew Wallace on 6/17/16.
//
//

#include "BeatMarker.hpp"


void BeatMarker::setup(float x, float y){
    
    pos.set(x,y);
    
    normSize = 10;
    pulseSize = normSize * 2;
    
    pulseTime = 0.15;
    pulseTimer = pulseTime;
    
    beingCleared = false;
    fallY = 0;
    fallDist = ofGetHeight() - y + pulseSize + 10;
    
    clearPauseRange = 0.1;
    fallTime = 0.3;
    pauseTime = fallTime + 0.2;
    growTime = pauseTime + 0.4;
    
    growHighScale = 1.5;
    
    curScale = 1;
    curSize = normSize;
}

void BeatMarker::update(float deltaTime){
    pulseTimer += deltaTime;
    
    curSize = normSize;
    
    if (pulseTimer < pulseTime && !beingCleared){
        float prc = pulseTimer / pulseTime;
        prc = powf(prc, 2);
        curSize = (1-prc) * pulseSize + prc * normSize;
    }
    
    if (beingCleared){
        clearTimer += deltaTime;
        
        if (clearTimer < 0){
            fallY = 0;
            curScale  = 1;
        }
        
        else if (clearTimer > 0 && clearTimer <= fallTime){
            float prc = clearTimer / fallTime;
            prc = powf(prc, 1.7);
            fallY = prc * fallDist;
            curScale = 1;
        }
        
        else if (clearTimer > fallTime && clearTimer <= pauseTime){
            curScale = 0;
        }
        
        else if (clearTimer > pauseTime && clearTimer <= growTime){
            float prc = (clearTimer-pauseTime)/(growTime-pauseTime);
            prc = powf(prc, 1.2);
            curScale = prc * growHighScale;
            fallY = 0;
        }
        
        else if (clearTimer > growTime){
            fallY = 0;
            beingCleared = false;
        }
        
        
    }else{
        //when we're not actively clearing, always be aproaching 1
        float xeno = 0.9;
        curScale = xeno * curScale + (1-xeno) * 1;
    }
    
}

void BeatMarker::triggerBeat(){
    pulseTimer = 0;
}

void BeatMarker::triggerClear(){
    beingCleared = true;
    clearTimer = -ofRandom(clearPauseRange);
}

void BeatMarker::draw(bool hasSound, bool isRecording){
    
    ofSetColor(0);
    ofSetLineWidth(1);
    ofSetCircleResolution(20);
    
    if (hasSound && isRecording)    ofFill();
    else                            ofNoFill();
    
    ofDrawCircle(pos.x, pos.y+fallY, curSize * curScale);
    
    //if it has a sound but we're not recording, draw a smaller circle inside
    if (hasSound && !isRecording){
        ofFill();
        ofDrawCircle(pos.x, pos.y+fallY, curSize * curScale * 0.25);
    }
    
    
}