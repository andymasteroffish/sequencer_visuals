//
//  CrystalHit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 6/8/16.
//
//

#include "CrystalHit.hpp"


void CrystalHit::setupCustom(){
    
    int boxSize = 60;
    int minDist = 20;

    ofVec2f top( ofRandom(-boxSize/2, boxSize/2), ofRandom(-minDist,-boxSize));
    ofVec2f botLeft( ofRandom(-minDist, -boxSize), ofRandom(minDist,boxSize));
    ofVec2f botRight(ofRandom(minDist,boxSize), ofRandom(minDist,boxSize));
    
    float centerGrowTime = 0.2;
    float centerPauseTime = 0.7;
    float centerDisconnectTime = 0.2;
    
    float edgeGrowTime = 0.4;
    float edgePauseTime = 0.7;
    float edgeDisconnectTime = 0.2;
    
    //first set of lines come from the center and move out to meet the corners
    lines[0].setup(0, 0, top.x, top.y, centerGrowTime, centerPauseTime, centerDisconnectTime);
    lines[1].setup(0, 0, botLeft.x, botLeft.y, centerGrowTime, centerPauseTime, centerDisconnectTime);
    lines[2].setup(0, 0, botRight.x, botRight.y, centerGrowTime, centerPauseTime, centerDisconnectTime);
    
    //second set lines are the edges
    lines[3].setup(top.x, top.y, botRight.x, botRight.y, edgeGrowTime, edgePauseTime, edgeDisconnectTime);
    lines[4].setup(top.x, top.y, botLeft.x, botLeft.y, edgeGrowTime, edgePauseTime, edgeDisconnectTime);

    lines[5].setup(botLeft.x, botLeft.y, botRight.x, botRight.y, edgeGrowTime, edgePauseTime+edgeDisconnectTime, edgeDisconnectTime);
    
    for (int i=0; i<6; i++){
        lines[i].setCurve(0.7, 1.4);
        //for the edge lines, we set their timer back so they don't draw right away
        if (i > 3){
            lines[i].timer = - centerGrowTime;
        }
    }
    
    pos.set( ofRandom(boxSize,gameW-boxSize), ofRandom(boxSize, gameH-boxSize));
    
    startAngle = ofRandom(TWO_PI);
    float angleAdjust = 180;
    if ( (int)pos.x % 2 == 0) angleAdjust *= -1;
    endAngle = startAngle + angleAdjust;
    angle = startAngle;
    
    
    
    
    //pop lines ot make it pop!
    numPopLines = 5;
    float angleStep = TWO_PI/(float)numPopLines;
    float popAngleStart = ofRandom(TWO_PI);
    float popStartDist = boxSize * 0.6;
    float popEndDist = boxSize * 1.1;
    for (int i=0; i<numPopLines; i++){
        float thisAngle = popAngleStart + angleStep * i;
        float nearX = cos(thisAngle) * popStartDist;
        float nearY = sin(thisAngle) * popStartDist;
        float farX = cos(thisAngle) * popEndDist;
        float farY = sin(thisAngle) * popEndDist;
        
        popLines[i].setup(nearX, nearY, farX, farY, 0, 0.1, 0.2);
    }
    
}

void CrystalHit::updateCustom(){
    
    for (int i=0; i<6; i++){
        lines[i].update(deltaTime);
    }
    
    for (int i=0; i<numPopLines; i++){
        popLines[i].update(deltaTime);
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
    
    if (lines[5].timer > lines[5].timeToDisconnect){
        killMe = true;
    }
    
}

void CrystalHit::draw(){
    
    ofPushMatrix();
    
    ofTranslate(pos);
    ofRotate(angle);
    
    ofSetColor(0);
    ofSetLineWidth(2);
    
    for (int i=0; i<6; i++){
        lines[i].draw();
    }
    
    for (int i=0; i<numPopLines; i++){
        popLines[i].draw();
    }
    
    ofPopMatrix();
    
}