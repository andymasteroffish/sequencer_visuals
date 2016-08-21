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
    
    fullColor.set(30);
    
    curFadeVal = 0;
    fadeSpeed = 700;
    
    text = "";
    font = NULL;
}

void TouchGeneralButton::setText(string _text, ofTrueTypeFont * _font){
    text = _text;
    font = _font;
}

void TouchGeneralButton::update(float deltaTime){
    curFadeVal -= fadeSpeed * deltaTime;
    curFadeVal = MAX(curFadeVal, 0);
}

void TouchGeneralButton::draw(){
    
    ofSetLineWidth(1);
    
    ofFill();
    ofSetColor(fullColor.r, fullColor.g, fullColor.b, curFadeVal);
    ofDrawRectangle(box);
    
    ofNoFill();
    ofSetColor(200,40);
    ofDrawRectangle(box);
    
    if (font != NULL){
        ofSetColor(0, 200);
        
        ofRectangle bounds = font->getStringBoundingBox(text, 0, 0);
        font->drawString(text, box.x+box.width/2-bounds.width/2, box.y+box.height/2+bounds.height/2);
        
    }
    
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