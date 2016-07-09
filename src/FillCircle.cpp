//
//  FillCircle.cpp
//  sequencer
//
//  Created by Andrew Wallace on 7/9/16.
//
//

#include "FillCircle.hpp"


void FillCircle::setup(float x, float y, ofRectangle _bounds){
    
    center.set(x, y);
    bounds = _bounds;
    
    curSize = 0;
    growthSpeed = 500;
    maxSize = bounds.width;
    
    numPoints = 30;
    angleStep = TWO_PI/numPoints;
    
    cornerSize = 100;
    topLeft.set(bounds.x+cornerSize, bounds.y+cornerSize);
    topRight.set(bounds.x+bounds.width-cornerSize, bounds.y+cornerSize);
    botLeft.set(bounds.x+cornerSize, bounds.y+bounds.height-cornerSize);
    botRight.set(bounds.x+bounds.width-cornerSize, bounds.y+bounds.height-cornerSize);
    
    isClosing = false;
}

void FillCircle::close(){
    isClosing = true;
    closeTimer = 0.1 + ofRandom(0.2);
}

void FillCircle::update(float deltaTime){
    
    if (!isClosing){
        if (curSize < maxSize){
            curSize += growthSpeed * deltaTime;
        }
    }else{
        closeTimer -= deltaTime;
        if (closeTimer < 0){
            float xeno = 0.9;
            curSize = curSize * xeno;
        }
    }
}

void FillCircle::draw(){
    
    ofFill();
    ofBeginShape();
    
    for (int i=0; i<numPoints; i++){
        float angle = angleStep * i;
        int x = center.x + cos(angle) * curSize;
        int y = center.y + sin(angle) * curSize;
        
        x = CLAMP(x, bounds.x, bounds.x+bounds.width);
        y = CLAMP(y, bounds.y, bounds.y+bounds.height);
        
        if (x < topLeft.x && y < topLeft.y){
            x = MAX(x, topLeft.x + cos(angle) * cornerSize);
            y = MAX(y, topLeft.y + sin(angle) * cornerSize);
        }
        
        if (x > topRight.x && y < topRight.y){
            x = MIN(x, topRight.x + cos(angle) * cornerSize);
            y = MAX(y, topRight.y + sin(angle) * cornerSize);
        }
        
        if (x < botLeft.x && y > botLeft.y){
            x = MAX(x, botLeft.x + cos(angle) * cornerSize);
            y = MIN(y, botLeft.y + sin(angle) * cornerSize);
        }
        
        if (x > botRight.x && y > botRight.y){
            x = MIN(x, botRight.x + cos(angle) * cornerSize);
            y = MIN(y, botRight.y + sin(angle) * cornerSize);
        }
        
        ofVertex(x, y);
    }
    
    ofEndShape(true);
    
}