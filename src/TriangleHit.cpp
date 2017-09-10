//
//  TriangleHit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#include "TriangleHit.hpp"


void TriangleHit::setupCustom(){
    
    float size = 80;
    
    float padding = size + 20;
    centerPos.set( ofRandom(padding, gameW-padding), ofRandom(padding,gameH-padding));
    
    
    if (arcadeMode){
        centerPos = getArcadePoint(arcadeModeDist);
    }
    
    lines.resize(3);
    float angleStep = TWO_PI/3;
    
    for (int i=0; i<3; i++){
        ofVec2f pntA, pntB;
        
        float angleA = angleStep*i;
        float angleB = angleStep*(i+1);
        
        pntA.x = cos(angleA) * size;
        pntA.y = sin(angleA) * size;
        
        pntB.x = cos(angleB) * size;
        pntB.y = sin(angleB) * size;
        
        lines[i].setup(pntA.x, pntA.y, pntB.x, pntB.y, 0.2, 0.4, 0.3);
        lines[i].setCurve(0.4, 2);
    }
    
    
    
    startAngle = ofRandom(360);
    endAngle = startAngle + 120;
    
    angle = startAngle;
    
    startScale = 1;
    endScale = 1.3;
    scale = startScale;
    
    //cout<<"start "<<startAngle<<endl;
    //cout<<"end   "<<endAngle<<endl<<endl;
    
}
void TriangleHit::updateCustom(){
    for (int i=0; i<lines.size(); i++){
        lines[i].update(deltaTime);
        if (lines[i].timer > lines[i].timeToDisconnect){
            killMe = true;
        }
    }
    
    //when the lines are not growing/shrinking, rotate
    if (lines[0].timer >= lines[0].timeToConnect && lines[0].timer < lines[0].timeToPause){
        float prc = (lines[0].timer-lines[0].timeToConnect) / (lines[0].timeToPause-lines[0].timeToConnect);
        prc = powf(prc, 1.5);
        angle = prc * endAngle + (1-prc) * startAngle;
    }
    
    //the the lines are disconencting, grow the whole shape
    if (lines[0].timer >= lines[0].timeToPause){
        float prc = (lines[0].timer-lines[0].timeToPause) / (lines[0].timeToDisconnect-lines[0].timeToPause);
        prc = powf(prc, 1.25);
        scale = prc * endScale + (1-prc)*startScale;
    }
    
}
void TriangleHit::draw(){
    ofSetColor(0);
    setLineWidth(2 );
    
    ofPushMatrix();
    
    float shakeRange =4;
    ofTranslate(ofRandom(-shakeRange,shakeRange), ofRandom(-shakeRange,shakeRange));
    
    ofTranslate(centerPos.x, centerPos.y);
    ofRotate(angle);
    ofScale(scale, scale);
    
    for (int i=0; i<lines.size(); i++){
        lines[i].draw();
    }
    
    ofPopMatrix();
    
}
