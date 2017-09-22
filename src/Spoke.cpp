//
//  Spoke.cpp
//  sequencer
//
//  Created by Andrew Wallace on 9/22/17.
//
//

#include "Spoke.hpp"

void Spoke::setup(float _targetDist, float _angle){
    setTargetDist(_targetDist);
    angle = _angle;
    
    anchorPos.set(0,0);
    
    circleSize = 5;
    
    frc = 0.10;
    
    float forceRange = 0.01;
    pullForce = 0.04 + ofRandom(-forceRange, forceRange);
    
    curDist = 0;
    curVel = 0;
    
    float updatesPerSec = 60;
    timeBetweenUpdates = 1.0/updatesPerSec;
    nextUpdateTime = 0;
}

void Spoke::setTargetDist(float _targetDist){
    targetDist = _targetDist;
}

void Spoke::update(float time){
    
    while(nextUpdateTime < time){
        fixedUpdate();
    }
    
}

void Spoke::fixedUpdate(){
    curVel *= (1.0-frc);
    
    float dist = targetDist - curDist;
    curVel += dist * pullForce;
    
    curDist += curVel;
    
    nextUpdateTime += timeBetweenUpdates;
}

void Spoke::drawLine(){
    float lineLength = curDist > 0 ? curDist-circleSize : curDist+circleSize;
    if (abs(lineLength) > circleSize){
        ofVec2f linePos;
        linePos.x = anchorPos.x + cos(angle) * lineLength;
        linePos.y = anchorPos.y + sin(angle) * lineLength;
        ofDrawLine(anchorPos, linePos);
    }
}
void Spoke::drawCircle(){
    ofVec2f circPos;
    circPos.x = anchorPos.x + cos(angle) * curDist;
    circPos.y = anchorPos.y + sin(angle) * curDist;
    ofDrawCircle(circPos.x, circPos.y, circleSize);
}
