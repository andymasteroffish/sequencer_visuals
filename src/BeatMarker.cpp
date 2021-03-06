//
//  BeatMarker.cpp
//  sequencer
//
//  Created by Andrew Wallace on 6/17/16.
//
//

#include "BeatMarker.hpp"


void BeatMarker::setup(float x, float _stepModeX, float y, int _whiteVal, bool usingIpad){
    
    whiteVal = _whiteVal;
    
    normX = x;
    stepModeX = _stepModeX;
    xenoX = 0.9;
    
    pos.set(x,y);
    
    normSize = 15;
    if (usingIpad)  normSize *= 2;
    
    maxTapDist = normSize * 1.5;
    
    pulseSize = normSize * 2;
    
    pulseTime = 0.15;
    pulseTimer = pulseTime;
    
    beingCleared = false;
    fallY = 0;
    fallDist = ofGetHeight() - y + pulseSize + 10;
    
    notRecordingYMaxDist = ofGetHeight() - y;
    
    clearPauseRange = 0.1;
    fallTime = 0.3;
    pauseTime = fallTime + 0.2;
    growTime = pauseTime + 0.4;
    
    growHighScale = 1.5;
    
    curScale = 1;
    curSize = normSize;
    
    notRecordingYOffset = 0;
    
    arcadeMode = false;
}

void BeatMarker::arcadeSetPos(float prc, float beatMarkerPrcDist){
    arcadeAngle = TWO_PI*prc;
    float dist = ofGetHeight() * beatMarkerPrcDist;
    
    normX = ofGetWidth()/2 + cos(arcadeAngle) * dist;
    pos.x = normX;
    pos.y = ofGetHeight()/2 + sin(arcadeAngle) * dist;
    
    arcadeMode = true;
}

void BeatMarker::update(float deltaTime, bool stepModeOn, float firstRunTimer){
    pulseTimer += deltaTime;
    
    float targetX = stepModeOn ? stepModeX : normX;
    pos.x = xenoX * pos.x + (1-xenoX) * targetX;
    
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
            hadSoundOnClear = false;
            hadOtherStepModeSoundOnClear = false;
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
    
    //when we're not recording, move to the bottom
    float notRecordingXeno = 0.9;
    if (isRecording){
        notRecordingYOffset = notRecordingYOffset * notRecordingXeno;
    }else{
        notRecordingYOffset = notRecordingYOffset * notRecordingXeno + notRecordingYMaxDist * (1-notRecordingXeno);
    }
    
    //when the app is run for the first time, we hide the dots
    float startEnterTime = 15;
    float endEnterTime = startEnterTime-2;
    firstRunYAdjust = 0;
    if (firstRunTimer > endEnterTime){
        float firstRunDist = 175;
        if (firstRunTimer < startEnterTime){
            float prc = (firstRunTimer-endEnterTime) / (startEnterTime-endEnterTime);
            firstRunYAdjust = firstRunDist * prc;
        }else{
            firstRunYAdjust = firstRunDist;
        }
        
        //cout<<"timer "<<firstRunTimer<<"  y adjust "<<firstRunYAdjust<<endl;
    }
    
}

bool BeatMarker::checkHit(int x, int y){
    if (ofDistSquared(x, y, pos.x, pos.y) < maxTapDist*maxTapDist){
        return true;
    }
    return false;
}

void BeatMarker::triggerBeat(){
    pulseTimer = 0;
}

void BeatMarker::triggerClear(){
    beingCleared = true;
    clearTimer = -ofRandom(clearPauseRange);
}

void BeatMarker::draw(bool hasSound, bool hasOtherStepModeSound, bool _isRecording){
    
    isRecording = _isRecording;
    
    ofSetColor(0);
    ofSetLineWidth(isOnTheOne ? 2 : 1);
    ofSetCircleResolution(20);
    
    if (beingCleared){
        hasSound = hadSoundOnClear;
        hasOtherStepModeSound = hadOtherStepModeSoundOnClear;
    }else{
        hadSoundOnClear = hasSound;
        hadOtherStepModeSoundOnClear = hasOtherStepModeSound;
    }
    
    if (hasSound && isRecording)    ofFill();
    else                            ofNoFill();
    
    float xPos = pos.x;
    float yOffsets = fallY+notRecordingYOffset+firstRunYAdjust;
    float yPos = pos.y+yOffsets;
    
    if (arcadeMode){
        xPos = pos.x + cos(arcadeAngle) * yOffsets;
        yPos = pos.y + sin(arcadeAngle) * yOffsets;
    }
    
    float bonusWidth = 0;
    if (isOnTheOne && hasSound && isRecording){
        bonusWidth = curSize * 0.2f;
    }
    
    ofDrawCircle(xPos, yPos, (curSize+bonusWidth) * curScale);
    
    //if it has a sound but we're not recording, draw a smaller circle inside
    if ( (hasSound && !isRecording) || hasOtherStepModeSound){
        ofFill();
        if (hasOtherStepModeSound && hasSound){
            ofSetColor(whiteVal);
        }
        ofDrawCircle(pos.x, yPos, curSize * curScale * 0.5);
    }
    
    
    //testing
//    ofSetColor(255,0,0);
//    ofNoFill();
//    ofDrawCircle(pos.x, pos.y, maxTapDist);
    
}
