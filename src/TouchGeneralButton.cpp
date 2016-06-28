//
//  TouchGeneralButton.cpp
//  sequncer
//
//  Created by Andrew Wallace on 6/28/16.
//
//

#include "TouchGeneralButton.hpp"

void TouchGeneralButton::setup(int _x, int _y, int _width, int _height){
//    pos.set(_x, _y);
//    width = _width;
//    height = _height;
    
    box.set(_x, _y, _width, _height);
    
    fullColor.set(100);
    
    curFadeVal = 0;
    fadeSpeed = 700;
}

void TouchGeneralButton::update(float deltaTime){
    curFadeVal -= fadeSpeed * deltaTime;
    curFadeVal = MAX(curFadeVal, 0);
}

void TouchGeneralButton::draw(){
    
    ofFill();
    ofSetColor(fullColor.r, fullColor.g, fullColor.b, curFadeVal);
    ofDrawRectangle(box);
    
    ofNoFill();
    ofSetColor(200,40);
    ofDrawRectangle(box);
    
//    ofSetColor(0);
//    ofDrawBitmapString(ofToString(box.x), box.x+10, box.y + 100);
    
}

bool TouchGeneralButton::checkHit(int x, int y){
    
    if (box.inside(x, y)){
        curFadeVal = 255;
        return  true;
    }
    
    return false;
}