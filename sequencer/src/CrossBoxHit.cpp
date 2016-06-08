//
//  CrossBoxHit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 6/8/16.
//
//

#include "CrossBoxHit.hpp"


void CrossBoxHit::setupCustom(){
    
    int boxSize = 35;
    
//    ofVec2f topLeft(-boxSize, -boxSize);
//    ofVec2f topRight(boxSize, -boxSize);
//    ofVec2f botLeft(-boxSize, boxSize);
//    ofVec2f botRight(boxSize, boxSize);
    
    //ofVec2f top( ofRandom(-boxSize/2, boxSize/2), ofRandom(0,-boxSize));
    
    ofVec2f topLeft(ofRandom(0, -boxSize), ofRandom(0,-boxSize));
    ofVec2f topRight(ofRandom(0,boxSize), ofRandom(0,-boxSize));
    ofVec2f botLeft(ofRandom(0,-boxSize), ofRandom(0,boxSize));
    ofVec2f botRight(ofRandom(0,boxSize), ofRandom(0,boxSize));
    
    float centerGrowTime = 0.2;
    float centerPauseTime = 0.7;
    float centerDisconnectTime = 0.2;
    
    float edgeGrowTime = 0.4;
    float edgePauseTime = 0.7;
    float edgeDisconnectTime = 0.2;
    
    //first 4 lines come from the center and move out to meet the corners
    lines[0].setup(0, 0, topLeft.x, topLeft.y, centerGrowTime, centerPauseTime, centerDisconnectTime);
    lines[1].setup(0, 0, topRight.x, topRight.y, centerGrowTime, centerPauseTime, centerDisconnectTime);
    lines[2].setup(0, 0, botLeft.x, botLeft.y, centerGrowTime, centerPauseTime, centerDisconnectTime);
    lines[3].setup(0, 0, botRight.x, botRight.y, centerGrowTime, centerPauseTime, centerDisconnectTime);
    
    //second 4 lines are the edges
    lines[4].setup(topRight.x, topRight.y, topLeft.x, topLeft.y, edgeGrowTime, edgePauseTime, edgeDisconnectTime);
    lines[5].setup(botRight.x, botRight.y, topRight.x, topRight.y, edgeGrowTime, edgePauseTime, edgeDisconnectTime);
    lines[6].setup(botLeft.x, botLeft.y, botRight.x, botRight.y, edgeGrowTime, edgePauseTime, edgeDisconnectTime);
    lines[7].setup(topLeft.x, topLeft.y, botLeft.x, botLeft.y, edgeGrowTime, edgePauseTime, edgeDisconnectTime);
    
    for (int i=0; i<8; i++){
        lines[i].setCurve(0.7, 1.4);
        //for the edge lines, we set their timer back so they don't draw right away
        if (i > 3){
            lines[i].timer = - centerGrowTime;
        }
    }
    
    startAngle = 0;
    endAngle = startAngle + 180;
    angle = startAngle;
    
    
    pos.set( ofRandom(boxSize,gameW-boxSize), ofRandom(boxSize, gameH-boxSize));
    
}

void CrossBoxHit::updateCustom(){
    
    for (int i=0; i<8; i++){
        lines[i].update(deltaTime);
    }
    
    //rotate this thang
    if (lines[0].timer > lines[0].timeToConnect && lines[0].timer < lines[0].timeToPause){
        float prc = (lines[0].timer-lines[0].timeToConnect) / (lines[0].timeToPause-lines[0].timeToConnect);
        prc = powf (prc, 2);
        angle = prc * endAngle + (1-prc) * startAngle;
    }
    else if (lines[0].timer >= lines[0].timeToPause){
        angle = endAngle;
    }
    
    if (lines[7].timer > lines[7].timeToDisconnect){
        killMe = true;
    }
    
}

void CrossBoxHit::draw(){
    
    ofPushMatrix();
    
    ofTranslate(pos);
    ofRotate(angle);
    
    ofSetColor(0);
    ofSetLineWidth(2);
    for (int i=0; i<8; i++){
        lines[i].draw();
    }
    
    ofPopMatrix();
    
}