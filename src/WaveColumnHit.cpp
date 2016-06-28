//
//  WaveColumnHit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 6/8/16.
//
//

#include "WaveColumnHit.hpp"


void WaveColumnHit::setupCustom(){
    
    
    float boxW = 60;
    float boxH = 130;
    
    pos.set( ofRandom(boxW, gameW-boxW), ofRandom(boxH, gameH-boxH));
    
    angle = 45;
    if (ofRandomuf() > 0.5) angle *= -1;
    
    int pntSpacing = 2;
    int numVerticalPointsPerSide = boxH/pntSpacing;
    
    numPoints = numVerticalPointsPerSide * 2;
    
    pnts.resize(numPoints);
    startPnts.resize(numPoints);
    basePnts.resize(numPoints);
    endPoints.resize(numPoints);
    
    timer = -0.1;
    
    growTime = 0.3;
    waveStartTime = growTime + 0.3;
    pauseTime = waveStartTime + 0.7;
    shrinkTime = pauseTime + 0.4;
    
    fullWaveAmount = 10;
    waveZoom = 0.3;
    waveSpeed = 5;
    
    //set up the base points, the actual rectangle
    for (int i=0; i<numVerticalPointsPerSide; i++){
        basePnts[i].x = -boxW/2;
        basePnts[i].y = -boxH/2 + pntSpacing * i;
        
        basePnts[numVerticalPointsPerSide+i].x = boxW/2;
        basePnts[numVerticalPointsPerSide+i].y = boxH/2 - pntSpacing * (i+1);
    }
    
    //setup the circle that they start in
    float angleStep = TWO_PI/numPoints;
    float startDist = boxW * 0.4;
    for (int i=0; i<numPoints; i++){
        float thisAngle = -PI/2 + angleStep * i;
        startPnts[i].x = -cos(thisAngle) * startDist;
        startPnts[i].y = sin(thisAngle) * startDist;
        
        pnts[i] = startPnts[i];
    }
    
    //setup the line that they will end in
    for (int i=0; i<numPoints; i++){
        endPoints[i].x = startPnts[i].x;
        endPoints[i].y = 0;
    }
    
    
    
    
}

void WaveColumnHit::updateCustom(){
    
    if (timer >= 0 && timer < growTime){
        float prc = timer/growTime;
        prc = powf (prc, 2);
        for (int i=0; i<numPoints; i++){
            pnts[i] = prc * basePnts[i] + (1-prc) * startPnts[i];
        }
    }
    else if (timer >= growTime && timer < pauseTime){
        for (int i=0; i<numPoints; i++){
            pnts[i] = basePnts[i];
        }
    }
    else if (timer >= pauseTime && timer < shrinkTime){
        float prc = (timer-pauseTime)/(shrinkTime-pauseTime);
        prc = powf (prc, 1.5);
        for (int i=0; i<numPoints; i++){
            pnts[i] = prc * endPoints[i] + (1-prc) * basePnts[i];
        }
    }
    else if (timer > shrinkTime){
        killMe = true;
    }
    
    
    //once we hit the shape, we start wiggling!
    if (timer > growTime){
        float waveAmount = fullWaveAmount;
        if (timer < waveStartTime){
            float prc = (timer-growTime) / (waveStartTime-growTime);
            waveAmount = prc * fullWaveAmount;
        }
        
        //go through each point starting at the tops
        for (int i=0; i<numPoints/2; i++){
            float shift = sin( (float)i*waveZoom + ofGetElapsedTimef()*waveSpeed) * waveAmount;
            pnts[i].x               += shift;
            pnts[numPoints/2+i].x   += shift;
        }
    }
    
}

void WaveColumnHit::draw(){
    
    ofPushMatrix();
    
    ofTranslate(pos);
    ofRotate(angle);
    
    ofSetColor(0);
    ofSetLineWidth(2);
    for (int i=0; i<numPoints-1; i++){
        ofDrawLine(pnts[i], pnts[i+1]);
    }
    ofDrawLine(pnts[0], pnts[ numPoints-1]);
    
    ofPopMatrix();
    
}