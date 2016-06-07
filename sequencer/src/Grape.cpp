//
//  Grape.cpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#include "Grape.hpp"


void Grape::setup(ofVec2f _pos, float delayTime){
    pos = _pos;
    
    growTime = 0.2;
    pauseTime = growTime+0.35;
    popTime = pauseTime+0.1;
    
    startSize = 5;
    normSize = 30;
    popSize = normSize + 20;
    
    setCurve(0.6, 1, 2);
    
    timer = -delayTime;
    
    //set up the pop lines
    float nearDist = popSize;
    float farDist = popSize + 15;
    int numLines = ofRandom(6,9);
    float angleStep = TWO_PI/(float)numLines;
    float startAngle = ofRandom(angleStep);
    popLines.resize(numLines);
    
    for (int i=0; i<numLines; i++){
        
        float angle = startAngle + angleStep * i;
        
        float x1 = pos.x + cos(angle) * nearDist;
        float y1 = pos.y + sin(angle) * nearDist;
        
        float x2 = pos.x + cos(angle) * farDist;
        float y2 = pos.y + sin(angle) * farDist;
        
        popLines[i].setup(x1, y1, x2, y2, 0, 0.1, 0.3);
        popLines[i].setCurve(lineCurve, lineCurve);
        
        
    }
    
    
}
void Grape::setCurve(float grow, float pop, float line){
    growCurve = grow;
    popCurve = pop;
    lineCurve = line;
}
void Grape::update(float deltaTime){
    timer += deltaTime;
    
    if (timer < 0){
        return;
    }
    
    if (timer < growTime){
        float prc = timer/growTime;
        prc = powf(prc, growCurve);
        size = prc * normSize + (1-prc) * startSize;
    }
    else if (timer >= growTime && timer < pauseTime){
        size = normSize;
    }
    else if (timer <= popTime){
        float prc = (timer-pauseTime)/(popTime-pauseTime);
        prc = powf(prc, popCurve);
        size = prc * popSize + (1-prc) * normSize;
    }
    
    
    if (timer > popTime){
        for (int i=0; i<popLines.size(); i++){
            popLines[i].update(deltaTime);
        }
    }
    
}
void Grape::draw(){
    if (timer < 0){
        return;
    }
    
    ofNoFill();
    ofSetColor(0);
    ofSetLineWidth(1);
    ofSetCircleResolution(30);
    if (timer < popTime){
        ofDrawCircle(pos.x, pos.y, size);
    }
    else{
        for (int i=0; i<popLines.size(); i++){
            popLines[i].draw();
        }
        
    }
    
}