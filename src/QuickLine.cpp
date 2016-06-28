//
//  QuickLine.cpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#include "QuickLine.hpp"

void QuickLine::setup(float x1, float y1, float x2, float y2, float connectTime, float pauseTime, float disconnectTime){
    
    pntA.set(x1, y1);
    pntB.set(x2, y2);
    
    timeToConnect = connectTime;
    timeToPause = pauseTime+timeToConnect;
    timeToDisconnect = disconnectTime+timeToPause;
    
    setCurve(1,1);
}

void QuickLine::setCurve(float connect, float disconnect){
    curveConnect = connect;
    curveDisconnect = disconnect;
}

void QuickLine::update(float deltaTime){
    timer += deltaTime;
}

void QuickLine::draw(){
    
    //connect
    if (timer >= 0 && timer < timeToConnect){
        float prc = timer / timeToConnect;
        prc = powf(prc, curveConnect);
        
        ofVec2f endPoint = prc * pntB + (1-prc) * pntA;
        
        ofDrawLine(pntA, endPoint);
    }
    
    //pause
    else if (timer >= timeToConnect && timer < timeToPause){
        ofDrawLine(pntA, pntB);
    }
    
    //disconnect
    else if (timer >= timeToPause && timer < timeToDisconnect){
        float prc = (timer-timeToPause) / (timeToDisconnect-timeToPause);
        prc = powf(prc, curveDisconnect);
        
        ofVec2f startPoint = prc * pntB + (1-prc) * pntA;
        
        ofDrawLine(startPoint, pntB);
    }
    
    
}