//
//  DrunkTriangleHit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 6/7/16.
//
//

#include "DrunkTriangleHit.hpp"


void DrunkTriangleHit::setupCustom(){
    
    numSteps = 15;
    stepDist = 18;
    
    float padding = 100;
    startPos.set( ofRandom(padding, gameW-padding), ofRandom(padding, gameH-padding));
    
    growTime = 0.2;
    normalizeTime = growTime + 0.2;
    pauseTime = normalizeTime + 1.2;
    growBeforeDeathTime = pauseTime + 0.2;
    shrinkTime = growBeforeDeathTime + 0.3;
    
    timerOffset = 0.2;
    
    normalSize = 8;
    largeSize = normalSize * 1.5;
    
    angles.resize(numSteps);
    sizes.resize(numSteps);
    
    float curAngle = ofRandom(TWO_PI);
    float angleRange = PI/4;
    for (int i=0; i<numSteps; i++){
        sizes[i] = 0;
        angles[i] = curAngle;
        curAngle += ofRandom(-angleRange, angleRange);
    }
    
}
void DrunkTriangleHit::updateCustom(){
    for (int i=0; i<numSteps; i++){
        
        int offsetVal = floor( (float)i / 2);
        float thisTimer = growTime + timer - timerOffset * offsetVal;
        
        if (thisTimer < 0){
            sizes[i] = 0;
        }
        else if (thisTimer >= 0 && thisTimer < growTime){
            float prc = thisTimer/growTime;
            prc = powf(prc, 1.5);
            sizes[i] = prc * largeSize;
        }
        else if (thisTimer >= growTime && thisTimer < normalizeTime){
            float prc = (thisTimer-growTime)/(normalizeTime-growTime);
            prc = powf(prc, 1.2);
            sizes[i] = prc * normalSize + (1-prc) * largeSize;
        }
        else if (thisTimer >= normalizeTime && thisTimer < pauseTime){
            sizes[i] = normalSize;
        }
        else if (thisTimer >= pauseTime && thisTimer < growBeforeDeathTime){
            float prc = (thisTimer-pauseTime)/(growBeforeDeathTime-pauseTime);
            prc = powf(prc, 0.7);
            sizes[i] = prc * largeSize + (1-prc) * normalSize;
        }
        else if (thisTimer >= growBeforeDeathTime && thisTimer < shrinkTime){
            float prc = (thisTimer-growBeforeDeathTime)/(shrinkTime-growBeforeDeathTime);
            prc = powf(prc, 2);
            sizes[i] = (1-prc) * largeSize;
        }
        else {
            sizes[i] = 0;
            if (i == numSteps-1){
                killMe = true;
            }
        }
    }
}
    
    
void DrunkTriangleHit::draw(){
    
    ofSetColor(0);
    ofSetLineWidth(2);
    ofSetCircleResolution(3);
    
    ofNoFill();
    
    ofVec2f curPos = startPos;
    for (int i=0; i<numSteps; i++){
        
        curPos.x += cos(angles[i]) * stepDist;
        curPos.y += sin(angles[i]) * stepDist;
        
        ofPushMatrix();
            
        ofTranslate(curPos.x, curPos.y);
        ofRotate( ofRadToDeg(angles[i]));
        
        ofDrawCircle(0, 0, sizes[i]);
        
        ofPopMatrix();
        
        
    }
    
}