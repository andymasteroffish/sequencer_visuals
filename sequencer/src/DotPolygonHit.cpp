//
//  DotPolygonHit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 6/7/16.
//
//

#include "DotPolygonHit.hpp"


void DotPolygonHit::setupCustom(){
    
    numPoints = 5;
    
    float range = 100;
    ofVec2f centerPos(ofRandom(range,gameW-range), ofRandom(range,gameH-range));
    
    normalSize = 10;
    largeSize = normalSize * 1.2;
    startSize = 2;
    
    growTime = 0.15;
    normalizeTime = growTime+0.1;
    
    //these two are just their own time, not based on the previous values
    preDeathGrowTime = 0.1;
    shrinkTime = 0.3;
    
    points.resize(numPoints);
    pointSize.resize(numPoints);
    startDeathTimes.resize(numPoints);
    lines.resize(numPoints);
    
    float basePauseTime = 1.5;
    float deathSpacing = 0.2;
    
    float curAngle = ofRandom(TWO_PI);
    
    //setup dots
    for (int i=0; i<numPoints; i++){
        startDeathTimes[i] = normalizeTime + basePauseTime + (float)i * deathSpacing;
        
        float thisDist = ofRandom(10,range);
        curAngle += ofRandom(PI*0.2,PI*0.5);
        
        points[i].x = centerPos.x + cos(curAngle) * thisDist;
        points[i].y = centerPos.y + sin(curAngle) * thisDist;
        
        pointSize[i] = startSize;
    }
    
    //setup lines
    for (int i=0; i<numPoints-1; i++){
        ofVec2f lineEnd;
        lineEnd = points[i+1];
        
        float disconnectTime = startDeathTimes[i+1]-startDeathTimes[i];
        
        lines[i].setup(points[i].x, points[i].y, lineEnd.x, lineEnd.y, 0, startDeathTimes[i], disconnectTime);
        lines[i].setCurve(1, 2);
    }
    //last line is special and links to the start
    float disconnectTime = startDeathTimes[1]-startDeathTimes[0];
    lines[numPoints-1].setup(points[numPoints-1].x, points[numPoints-1].y, points[0].x, points[0].y, 0, startDeathTimes[0], disconnectTime);
    lines[numPoints-1].setCurve(1, 2);
    
}

void DotPolygonHit::updateCustom(){
    
    for (int i=0; i< numPoints; i++){
        
        float myPreDeathGrowTime = startDeathTimes[i]+preDeathGrowTime;
        float myShrinkTime = myPreDeathGrowTime + shrinkTime;
        
        if (timer <= growTime){
            float prc = timer/growTime;
            prc = powf(prc, 0.5);
            pointSize[i] = prc * largeSize + (1-prc) * startSize;
        }
        else if (timer > growTime && timer <= normalizeTime){
            float prc = (timer-growTime)/(normalizeTime-growTime);
            prc = powf(prc, 1.3);
            pointSize[i] = prc * normalSize + (1-prc) * largeSize;
        }
        else if (timer > normalizeTime && timer <= startDeathTimes[i]){
            pointSize[i] = normalSize + sin(ofGetElapsedTimef()) * 2;
        }
        else if (timer > startDeathTimes[i] && timer <= myPreDeathGrowTime){
            float prc = (timer-startDeathTimes[i])/(myPreDeathGrowTime-startDeathTimes[i]);
            prc = powf(prc, 0.7);
            pointSize[i] = prc * largeSize + (1-prc) * normalSize;
        }
        else if (timer > myPreDeathGrowTime && timer <= myShrinkTime){
            float prc = (timer-myPreDeathGrowTime)/(myShrinkTime-myPreDeathGrowTime);
            prc = powf(prc, 0.8);
            pointSize[i] = (1-prc) * largeSize;
        }
        else{
            pointSize[i] = 0;
        }
        
        lines[i].update(deltaTime);
        
    }
    
    if (pointSize[numPoints-1] <= 0){
        killMe = true;
    }
    
    
}

void DotPolygonHit::draw(){
    
    
    ofSetColor(0);
    ofSetLineWidth(1);
    ofSetCircleResolution(20);
    for (int i=0; i<numPoints; i++){
        lines[i].draw();
    }
    
    ofFill();
    for (int i=0; i<numPoints; i++){
        ofSetColor(0);
        ofDrawCircle(points[i].x, points[i].y, pointSize[i]);
        ofSetColor(255);
        ofDrawCircle(points[i].x, points[i].y, pointSize[i]*0.6);
    }
}



