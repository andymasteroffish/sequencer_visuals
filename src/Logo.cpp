//
//  Logo.cpp
//  sequencer
//
//  Created by Andrew Wallace on 3/11/17.
//
//

#include "Logo.hpp"

void Logo::setup(int _whiteVal, bool usingiPad, ofTrueTypeFont * _font){
    
    growTime = 1.5;
    pauseTime = 6;
    fadeTime = 0.4;
    
    whiteVal = _whiteVal;
    
    font = _font;
    
    masterScale = 1;
    if (usingiPad)  masterScale = 1.5;
    
    timer = 0;
    
    logoPic.load("logo_quick_crop.png");
    
    logoPos.set(ofGetWidth()*0.5, ofGetHeight()*0.5);
}

void Logo::update(float deltaTime){
    timer += deltaTime;
    
    if (timer < growTime){
        prcComplete = timer/growTime;
    }else if (timer < growTime + pauseTime){
        prcComplete = 1;
    }
    else {
        prcComplete = 1.0f - (timer-(growTime+pauseTime)) / fadeTime;
    }
    prcComplete = CLAMP(prcComplete, 0, 1);
    
}

void Logo::draw(){
    
    if (prcComplete <= 0){
        return;
    }
    
    ofEnableAlphaBlending();
    
    ofPushMatrix();
    
    ofTranslate(logoPos.x, logoPos.y);
    
    ofScale(masterScale, masterScale);
    
    drawTestImage();
    
    drawHeadphoneMessage();
    
    ofPopMatrix();
    
    ofDisableAlphaBlending();
}

void Logo::drawTestImage(){
    ofSetColor(whiteVal, 255 * prcComplete);
    
    ofPushMatrix();
    ofTranslate(0, -ofGetHeight()*0.2);
    ofScale(0.5, 0.5);
    logoPic.draw(-logoPic.getWidth()/2, -logoPic.getHeight()/2);
    ofPopMatrix();
}

void Logo::drawHeadphoneMessage(){
    
    float prcToShrink = 0.75;
    
    float curHeadphoneScale = 1.2 + sin(ofGetElapsedTimef()) * 0.1;
        
    if (prcComplete < prcToShrink){
        float prc = ofMap(prcComplete, 0, prcToShrink, 0, 1);
        prc = powf(prc, 2);
        curHeadphoneScale *= prc;
    }
    
    ofPushMatrix();
    ofTranslate(0, ofGetHeight()*0.1);
    ofScale(curHeadphoneScale, curHeadphoneScale);
    
    string headphoneMessage = "Headphones recommended";
    ofRectangle headphoneRect = font->getStringBoundingBox(headphoneMessage, 0, 0);
    float headphoneTextW = headphoneRect.width;
    float headphoneTextH = headphoneRect.height;
    ofSetColor(0, 100);
    font->drawString("Headphones recomended", -headphoneTextW/2, headphoneTextH/4);
    
    ofPopMatrix();
}
