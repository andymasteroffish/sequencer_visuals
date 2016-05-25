//
//  SquareHit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 5/25/16.
//
//

#include "SquareHit.hpp"


void SquareHit::setupCustom(){
    
    float size = 60;
    
    pos.x = ofRandom(size, gameW-size);
    pos.y = ofRandom(size, gameH-size);
    
    float growTime = 0.1;
    float pauseTime = 0.7;
    float shrinkTime = 0.6;
    
    float curveIn = 0.6;
    float curveOut = 2;
    
    //top line
    lines[0].setup(0, 0, size, 0, growTime, pauseTime, shrinkTime);
    
    //left
    lines[1].setup(0, 0, 0, size, growTime, pauseTime, shrinkTime);
    
    //bottom
    lines[2].setup(0, size, size, size, growTime, pauseTime+growTime, shrinkTime);
    lines[2].timer = -growTime;
    
    //right
    lines[3].setup(size, 0, size, size, growTime, pauseTime+growTime, shrinkTime);
    lines[3].timer = -growTime;
    
    for (int i=0; i<4; i++){
        lines[i].setCurve(curveIn, curveOut);
    }
    
    
    startFallTime = growTime*2;
    fallTime = 0.5;
    angle = 0;
    angleEnd = 130;
    angleOvershoot = angleEnd + 15;
    angleOvershootTime = 0.1;
    
}

void SquareHit::updateCustom(){
    for (int i=0; i<4; i++){
        lines[i].update(deltaTime);
    }
    
    if (timer > startFallTime){
        float prc = (timer-startFallTime)/fallTime;
        prc = powf(prc, 2);
        angle = prc * angleOvershoot;
        
    }
    if (timer > startFallTime + fallTime){
        float prc = (timer-startFallTime-fallTime)/angleOvershootTime;
        prc = CLAMP(prc, 0 ,1);
        prc = powf(prc, 0.5);
        angle = prc * angleEnd + (1-prc)*angleOvershoot;
        //float xeno = 0.9;
        //angle = xeno*angle + (1-xeno)*angleEnd;
    }
}

void SquareHit::draw(){
    ofPushMatrix();
    ofTranslate(pos);
    
    ofRotate(angle);
    
    ofTranslate(0, -50);
    
    ofSetColor(0);
    ofSetLineWidth(2);
    
    for (int i=0; i<4; i++){
        lines[i].draw();
    }
    
    ofPopMatrix();
}