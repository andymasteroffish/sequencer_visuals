//
//  Buckshot.cpp
//  sequencer
//
//  Created by Andrew Wallace on 5/25/16.
//
//

#include "Buckshot.hpp"


void Buckshot::setup(float x, float y, float delayTime){//, float _lineWidthPrc){
    pos.set(x,y);
    timer = -delayTime;
    
    //lineWidthPrc = _lineWidthPrc;
    
    startTime = 0.3;
    pauseTime = startTime+0.6;
    popTime = pauseTime+0.3;
    
    startSize = 20;//10;
    normSize = 16;//8;
    popSize = -40;//-25;
    
    size = startSize;
    
    isDone = false;
}

void Buckshot::update(float deltaTime){
    timer += deltaTime;
    
    if (timer > 0 && timer <= startTime){
        float prc = timer/startTime;
        prc = powf(prc, 0.4);
        size = prc * normSize + (1-prc) * startSize;
    }
    if (timer > startTime && timer <= pauseTime){
        size = normSize;
    }
    if (timer > pauseTime && timer <= popTime){
        float prc = (timer-pauseTime)/(popTime-pauseTime);
        prc = powf(prc, 1.5);
        size = prc * popSize + (1-prc) * normSize;
    }
    
    if (timer > popTime){
        isDone = true;
    }
}

void Buckshot::draw(){
    
    if (timer < 0 || isDone){
        return;
    }
    
    ofSetColor(0);
    ofNoFill();
    ofSetCircleResolution(20);
    ofDrawCircle(pos.x, pos.y, size);
}
