//
//  Logo.cpp
//  sequencer
//
//  Created by Andrew Wallace on 3/11/17.
//
//

#include "Logo.hpp"

void Logo::setup(int _whiteVal, bool _usingiPad, ofTrueTypeFont * _font, ofTrueTypeFont * _fontSmall){
    
    usingiPad = _usingiPad;
    
    growTime = 1.5;
    pauseTime = 7;
    fadeTime = 0.4;
    
    whiteVal = _whiteVal;
    
    font = _font;
    fontSmall = _fontSmall;
    
    masterScale = 1;
    if (usingiPad)  masterScale *= 1.5;
    
    timer = 0;
    
    for (int i=0; i<LOGO_SIZE; i++){
        logoPics[i].load("logo/"+ofToString(i)+".png");
    }
    
    //logoPic.load("logo_quick_crop.png");
    
    logoPos.set(ofGetWidth()*0.5, ofGetHeight()*0.55);
}

void Logo::update(float deltaTime){
    timer += deltaTime;
    
    if (timer < growTime){
        //prcComplete = timer/growTime;
        //trying out having the animaitons start fileld in so they can be on the load screen
        prcComplete = 1;
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
    
    drawLogoImages();
    
    drawHeadphoneMessage();
    
    ofPopMatrix();
    
    ofDisableAlphaBlending();
}

void Logo::drawLogoImages(){
    ofEnableSmoothing();
    ofSetColor(whiteVal, 255 * prcComplete);
    
    float generalYOffset =  -150;//ofGetHeight()*0.2;
    
    float noiseSpeed = 0.3;
    float noiseRange = 10;
    
    for (int i=0; i<LOGO_SIZE; i++){
        ofVec2f pos;
        
        pos.x = ofNoise(ofGetElapsedTimef()*noiseSpeed, i, 20) * noiseRange;
        pos.y = ofNoise(ofGetElapsedTimef()*noiseSpeed, i, 100) * noiseRange;
        float noiseScale = MIN(1,timer/growTime);
        
        //noiseScale = 0; //KILL ME
        
        ofPushMatrix();
        ofTranslate(0,generalYOffset);
        ofTranslate(pos.x*noiseScale, pos.y*noiseScale);
        ofScale(0.75, 0.75);
        
        logoPics[i].draw(-logoPics[i].getWidth()/2, -logoPics[i].getHeight()/2);
    
        ofPopMatrix();
    }
    
    ofDisableSmoothing();
}

void Logo::drawHeadphoneMessage(){
    
    float prcToShrink = 0.75;
    
    float headphoneY = 150;
    float creditsY = 225;
    
    if (usingiPad){
        headphoneY = 175;
        creditsY = 275;
    }
    
    float curHeadphoneScale = 1.2 + sin(ofGetElapsedTimef()) * 0.1;
    //curHeadphoneScale = 1.2 + sin(0) * 0.1;   //KILL ME
        
    if (prcComplete < prcToShrink){
        float prc = ofMap(prcComplete, 0, prcToShrink, 0, 1);
        prc = powf(prc, 2);
        curHeadphoneScale *= prc;
    }
    
    ofPushMatrix();
    ofTranslate(0, headphoneY);//ofGetHeight()*0.2);
    ofScale(curHeadphoneScale, curHeadphoneScale);
    
    string headphoneMessage = "Headphones recommended";
    ofRectangle headphoneRect = font->getStringBoundingBox(headphoneMessage, 0, 0);
    float headphoneTextW = headphoneRect.width;
    float headphoneTextH = headphoneRect.height;
    ofSetColor(0, 100);
    font->drawString("Headphones recomended", -headphoneTextW/2, headphoneTextH/4);
    
    ofPopMatrix();
    
    
    ofSetColor(0, 100*prcComplete);
    string creditsMessage = "by Andy Wallace & Dan Friel";
    ofRectangle creditsRect = fontSmall->getStringBoundingBox(headphoneMessage, 0, 0);
    float creditTextW = creditsRect.width;
    fontSmall->drawString(creditsMessage, -creditTextW/2, creditsY);//ofGetHeight()*0.3);
}

//void Logo::drawCredits(){
//    
//    
//}
