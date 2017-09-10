//
//  Chaser.cpp
//  sequencer
//
//  Created by Andrew Wallace on 5/25/16.
//
//

#include "ChaserHit.hpp"


void ChaserHit::setupCustom(){
    
    numPoints = 30;
    
    points.resize(numPoints);
    
    fillTime = 0.5;
    
    angle = ofRandom(360);
    
    
    float range = 50;
    pos.x = ofRandom(range, gameW-range);
    pos.y = ofRandom(range, gameH-range);
    
    if (arcadeMode){
        pos = getArcadePoint(arcadeModeDist);
    }
    
    float angleStep = TWO_PI/numPoints;
    
    for (int i=0; i<numPoints; i++){
        float thisAngle = angleStep * i;
        points[i].x = cos(thisAngle) * range;
        points[i].y = sin(thisAngle) * range;
    }
    
    phase = 0;
    
}

void ChaserHit::updateCustom(){
    
    if (timer > fillTime){
        phase++;
        timer = 0;
        if (phase == 3){
            killMe = true;
        }
    }
    
}

void ChaserHit::draw(){
    
    float prc = timer/fillTime;
    
    ofSetColor(0);
    
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofRotate(angle);
    
    //flip it horizontal some of the time
    if ( (int)pos.x % 2 == 0){
        ofScale(-1,1);
    }
    
    //initial fill
    if (phase == 0){
        prc = powf(prc, 2);
        int pointsToFill = numPoints * prc;
        for (int i=0; i<pointsToFill && i <points.size(); i++){
            if (i == pointsToFill-1){
                setLineWidth(4);
            }else{
                setLineWidth(1);
            }
            
            if (i < points.size()-1){
                ofDrawLine(points[i], points[i+1]);
            }else{
                ofDrawLine(points[i], points[0]);
            }
        }
    }
    
    //moving around in a circle
    if (phase == 1){
        int highlightSection = numPoints * prc;
        for (int i=0; i<points.size(); i++){
            if (i == highlightSection){
                setLineWidth(4);
            }else{
                setLineWidth(1);
            }
            
            if (i < points.size()-1){
                ofDrawLine(points[i], points[i+1]);
            }else{
                ofDrawLine(points[i], points[0]);
            }
        }
    }
    
    //retracting it as we go
    if (phase == 2){
        prc = powf(prc, 0.5);
        int highlightSection = numPoints * prc;
        for (int i=highlightSection; i<points.size(); i++){
            if (i == highlightSection){
                setLineWidth(4);
            }else{
                setLineWidth(1);
            }
            
            if (i < points.size()-1){
                ofDrawLine(points[i], points[i+1]);
            }else{
                ofDrawLine(points[i], points[0]);
            }
        }
    }
    
    
    ofPopMatrix();
    
}
