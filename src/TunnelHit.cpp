//
//  TunnelHit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#include "TunnelHit.hpp"


void TunnelHit::setupCustom(){
    float rangeX = gameW * 0.2;
    float rangeY = gameH * 0.2;
    
    pos.x = gameW/2 + ofRandom(-rangeX, rangeX);
    pos.y = gameW/2 + ofRandom(-rangeX, rangeX);
    
    startSize = 70;
    smallSize = 10;
    bigSize = gameW;
    
    shrinkTime = 0.2;
    growTime = 0.7;
    
    size = startSize;
    //growthRate = 1500;
}
void TunnelHit::updateCustom(){
    
    
    if (timer < shrinkTime){
        float prc = timer/shrinkTime;
        prc = powf(prc, 0.5);
        size = prc * smallSize + (1-prc) * startSize;
    }else{
        float prc = (timer-shrinkTime) / growTime;
        prc = powf(prc, 0.8);
        size = prc * bigSize + (1-prc) * smallSize;
    }
    
    //size += growthRate * deltaTime;
    
    if (timer > growTime){
        killMe = true;
    }
    
}
void TunnelHit::draw(){
    ofSetCircleResolution(40);
    ofSetColor(0);
    ofSetLineWidth(2 * lineWidthPrc);
    ofNoFill();
    
    ofDrawCircle(pos.x, pos.y, size);
}