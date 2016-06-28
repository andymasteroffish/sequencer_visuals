//
//  SlashHit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 5/25/16.
//
//

#include "SlashHit.hpp"


void SlashHit::setupCustom(){
    
    float range = 100;
    
    float connectTime = 0.1;//2;
    float pauseTime = 0.5;
    float disconenctTime = 0.3;
    
    float padding = range * 2;
    pos.set(ofRandom(padding, gameW-padding), ofRandom(padding, gameH-padding));
    
    //get two points based on that point
    float angleA = ofRandom(TWO_PI);
    float angleB = angleA + ofRandom(TWO_PI * 0.2, TWO_PI * 0.4);
    
    float x1 = cos(angleA) * range;
    float y1 = sin(angleA) * range;
    
    float x2 = cos(angleB) * range;
    float y2 = sin(angleB) * range;
    
    lineA.setup(x1, y1, 0, 0, connectTime, pauseTime, disconenctTime);
    lineB.setup(0, 0, x2, y2, connectTime, pauseTime+0.2, disconenctTime);
    lineB.timer = -connectTime;
    
    float curveIn = 1.4;
    float curveOut = 1.4;
    lineA.setCurve(curveIn, curveOut);
    lineB.setCurve(curveIn, curveOut);
    
    circleStartGrowTime = 0.1;
    circleGrowTime = circleStartGrowTime+0.2;
    circlePauseTime = circleGrowTime + 0.7;
    circleShrinkTime = circlePauseTime + 0.4;
    
    
    
    circleSize = 0;
    circleNormSize = 10;
    circleBigSize = circleNormSize + 5;
    
}

void SlashHit::updateCustom(){
    
    lineA.update(deltaTime);
    lineB.update(deltaTime);
    
    if (timer < circleStartGrowTime){
        circleSize = 0;
    }
    else if (timer < circleGrowTime){
        float prc = (timer-circleStartGrowTime)/(circleGrowTime-circleStartGrowTime);
        prc = powf(prc, 1.8);
        circleSize = prc * circleBigSize;
    }
    else if (timer < circlePauseTime){
        float xeno = 0.9;
        circleSize = xeno*circleSize + (1-xeno) * circleNormSize;
    }
    else if (timer < circleShrinkTime){
        float prc = (timer-circlePauseTime)/(circleShrinkTime-circlePauseTime);
        prc = powf(prc, 0.6);
        circleSize = (1-prc) * circleBigSize;
    }
    else{
        killMe = true;
    }
    
}

void SlashHit::draw(){
    
    ofSetLineWidth(5);
    ofSetColor(0);
    
    ofPushMatrix();
    
    ofTranslate(pos.x, pos.y);
    
    ofFill();
    ofSetCircleResolution(20);
    ofDrawCircle(0,0, circleSize);
    
    lineA.draw();
    lineB.draw();
    
    ofSetColor(whiteVal);
    ofSetLineWidth(1);
    lineA.draw();
    lineB.draw();
    
    
    
    
    ofPopMatrix();
    
}