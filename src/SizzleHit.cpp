//
//  SizzleHit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 6/7/16.
//
//

#include "SizzleHit.hpp"


void SizzleHit::setupCustom(){
    
    numPoints = 15;
    
    spreadTime = 0.3;
    pauseTime = spreadTime + 0.7;
    dissipateTime = pauseTime + 0.5;
    
    points.resize(numPoints);
    targetPoints.resize(numPoints);
    basePoints.resize(numPoints);
    
    float curY = 0;
    for (int i=0; i<numPoints; i++){
        points[i].set(0,0);
        
        float xMin = 2 + 1 * i;
        float xMax = 5 + 5 * i;
        float xVal = ofRandom(xMin, xMax);
        if (i%2==1) xVal *= -1;
        
        float yMin = 1 + 1 * i;
        float yMax = 6 + 1 * i;
        curY -= ofRandom(yMin, yMax);
        
        basePoints[i].set(xVal, curY);
        targetPoints[i].set(xVal, curY);
    }
    
    pos.set( ofRandom(100,gameW-100), ofRandom(150, gameH-10));
    angle = 0;
    
    if (arcadeMode){
        pos = getArcadePoint(arcadeModeDist);
        angle = ofRandom(360);
    }
}

void SizzleHit::updateCustom(){
    
    //set where they should be based on noise
    float noiseRange = 8;
    float noiseSpeed = 5;
    for (int i=0; i<numPoints; i++){
        targetPoints[i] = basePoints[i] + (1-(ofNoise(i, ofGetElapsedTimef()*noiseSpeed)*2)) * noiseRange;
    }

    
    //actually set the points
    ofVec2f startPoint(0, points[numPoints/2].y);
    float prc = timer/spreadTime;
    prc = powf(prc, 0.2);
    if (prc > 1)    prc = 1;
    for (int i=0; i<numPoints; i++){
        points[i] = prc * targetPoints[i] + (1-prc)*startPoint;
    }
    
    if (timer > dissipateTime){
        killMe = true;
    }
    
}

void SizzleHit::draw(){
    
    ofPushMatrix();
    ofTranslate(pos);
    ofRotate(angle);
    
    ofSetColor(0);
    ofSetLineWidth(1 * lineWidthPrc);
    
    if (timer <= pauseTime){
        for (int i=0; i<numPoints-1; i++){
            ofDrawLine(points[i], points[i+1]);
        }
    }else{
        float prc = (timer-pauseTime)/(dissipateTime-pauseTime);
        prc = CLAMP(prc, 0, 1);
        prc = powf(prc, 2);
        //cout<<"prc "<<prc<<endl;
        for (int i=0; i<numPoints-1; i++){
            ofVec2f centerPos = (points[i]+points[i+1])/2;
            
            //draw a line prc of the way from the point to the center pos
            ofVec2f endA = prc * points[i] + (1-prc) * centerPos;
            ofVec2f endB = prc * points[i+1] + (1-prc) * centerPos;
            
            ofDrawLine(points[i], endA);
            ofDrawLine(points[i+1], endB);
        }
        
    }
    
    ofPopMatrix();
    
}
