//
//  ClapHit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 6/8/16.
//
//

#include "ClapHit.hpp"


void ClapHit::setupCustom(){
    
    angle = ofRandom(360);
    
    startScale = 3;
    impactTime = 0.16;
    
    pntStartDist = 70;
    pntDist = pntStartDist;
    
    firstClapTime = 0.2;
    secondClapTime = firstClapTime + 0.4;
    pauseTime = secondClapTime + 0.2;
    recedeTime = pauseTime + 0.4;
    
    startAngle = PI/4;
    pntAngle = startAngle;
    
    popCircleTime = 0.2;
    popCircleSize = pntStartDist * 0.5;
    popCircleDist = pntStartDist + popCircleSize * 0.75;
    leftPopCircleSize = 0;
    rightPopCircleSize = 0;
    
    pos.set( ofRandom(pntStartDist, gameW-pntStartDist), ofRandom(pntStartDist, gameH-pntStartDist));
}

void ClapHit::updateCustom(){
    
    //have it slam in
    if (timer < impactTime){
        float prc = timer/impactTime;
        curScale = prc * 1 + (1-prc) * startScale;
    }else if (timer > impactTime){
        curScale = 1;
    }
    
    //move the points around
    
    if (timer < firstClapTime){
        //go form startAngle to 0
        float prc = timer/firstClapTime;
        prc = powf(prc, 1.8);
        pntAngle = (1-prc) * startAngle;
    }
    else if (timer >= firstClapTime && timer < secondClapTime){
        //go from 0 to PI
        float prc = (timer-firstClapTime) / (secondClapTime-firstClapTime);
        prc = powf(prc, 1.8);
        pntAngle = prc * PI;
    }
    else if (timer >= secondClapTime){
        pntAngle = PI;
    }
    
    //pull the liens in at the end
    if (timer >= pauseTime){
        float prc = (timer-pauseTime)/(recedeTime-pauseTime);
        prc = powf(prc, 0.7);
        pntDist = prc * 0 + (1-prc) * pntStartDist;
    }
    
    if (timer >= recedeTime){
        killMe = true;
    }
    
    //adjust the points
    pntA.x = cos(pntAngle) * pntDist;
    pntA.y = sin(pntAngle) * pntDist;
    pntB.x = cos(-pntAngle) * pntDist;
    pntB.y = sin(-pntAngle) * pntDist;
    
    
    //mess with the pop circles
    leftPopCircleSize = 0;
    rightPopCircleSize = 0;
    if (timer > firstClapTime && timer < firstClapTime+popCircleTime){
        float prc = (timer-firstClapTime) / popCircleTime;
        prc = powf(prc, 0.9);
        rightPopCircleSize = (1-prc) * popCircleSize;
    }
    if (timer > secondClapTime && timer < secondClapTime+popCircleTime){
        float prc = (timer-secondClapTime) / popCircleTime;
        prc = powf(prc, 0.9);
        leftPopCircleSize = (1-prc) * popCircleSize * 0.7;  //just making it a bit smaller
    }
    
    
    
}

void ClapHit::draw(){
    
    ofPushMatrix();
    
    ofTranslate(pos);
    ofScale(curScale, curScale);
    ofRotate(angle);
    
    //draw the lines
    ofSetColor(0);
    ofSetLineWidth(3 * lineWidthPrc);
    ofDrawLine(0, 0, pntA.x, pntA.y);
    ofDrawLine(0, 0, pntB.x, pntB.y);
    
    //draw the pop circles
    ofNoFill();
    ofSetLineWidth(2 * lineWidthPrc);
    ofSetCircleResolution(20);
    if (leftPopCircleSize > 0){
        ofDrawCircle(-popCircleDist, 0, leftPopCircleSize);
    }
    if (rightPopCircleSize > 0){
        ofDrawCircle(popCircleDist, 0, rightPopCircleSize);
    }
    
    ofPopMatrix();
}