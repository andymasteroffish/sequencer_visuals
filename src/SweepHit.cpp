//
//  SweepHit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#include "SweepHit.hpp"


void SweepHit::setupCustom(){
    dir = ofRandomuf() > 0.5 ? 1 : -1;
    
    sweepTime = 0.7;
    trailLength = 30;
    
    float startDist = 100;
    if (dir == -1){
        startX = gameW-startDist;
        endX = -trailLength * 2;
    }else{
        startX = startDist;
        endX = gameW + trailLength * 2;
    }
    
    
    xPos = startX;
    
    //speed = 2400;
}
void SweepHit::updateCustom(){
    
    float prc = timer/sweepTime;
    prc = powf(prc, 2);
    xPos = prc * endX + (1-prc) *startX;
    //xPos += speed * deltaTime * dir;
    
//    if (xPos < -trailLength*2 || xPos > gameW + trailLength*2){
//        killMe = true;
//    }

    if (timer > sweepTime){
        killMe = true;
    }
}
void SweepHit::draw(){
    
    ofSetColor(0);
    ofSetLineWidth(1 * lineWidthPrc);
    
    ofDrawLine(xPos, 0, xPos, gameH);
    
    for (int i=0; i<trailLength; i++){
        int thisX = xPos + i * 2 * -dir;
        int numDots = ofMap(i, 0, trailLength, 20, 1);
        
        for (int k=0; k<numDots; k++){
            float thisHeight = ofRandom(10, 100);
            float thisY = ofRandom(-thisHeight,gameH+thisHeight);
            
            ofDrawLine(thisX, thisY, thisX, thisY+thisHeight);
        }
    }
    
}