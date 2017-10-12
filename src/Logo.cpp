//
//  Logo.cpp
//  sequencer
//
//  Created by Andrew Wallace on 3/11/17.
//
//

#include "Logo.hpp"

void Logo::setup(int _whiteVal, bool _usingiPad, ofTrueTypeFont * _font, ofTrueTypeFont * _fontSmall, bool _arcadeMode){
    
    usingiPad = _usingiPad;
    arcadeMode = _arcadeMode;
    
    growTime = 1.5;
    pauseTime = 7;
    fadeTime = 0.4;

	arcadeAngle = 0;
	nextArcadeAngle = 0;
    
    whiteVal = _whiteVal;
    
    font = _font;
    fontSmall = _fontSmall;
    
    masterScale = 1;
    //if (usingiPad)  masterScale *= 1.5;
    
    timer = 0;
    
    for (int i=0; i<LOGO_SIZE; i++){
        logoPics[i].load("logo/"+ofToString(i)+".png");
    }
    
    //logoPic.load("logo_quick_crop.png");
    
    setPos();
}

void Logo::setPos(){
     logoPos.set(ofGetWidth()*0.5, ofGetHeight()*0.55);
    
    if (arcadeMode){
		logoPos.set(ofGetWidth()*0.5, ofGetHeight()*0.5);
        masterScale = (float)ofGetHeight() / 1100.0f; // ~* MaGiC nUmBeR *~
        //cout<<"SET IT NOW "<<masterScale<<endl;
    }
}

void Logo::arcadeReset() {
	timer = -1;
	arcadeAngle = nextArcadeAngle;// ((int)ofRandom(0, 4)) * 90;
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
	if (arcadeMode) {
		ofRotate(arcadeAngle);
	}
    
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
    
    float logoScale = 0.75;
    if (usingiPad){
        logoScale = 1.2;
    }
    
    for (int i=0; i<LOGO_SIZE; i++){
        ofVec2f pos;
        
        pos.x = ofNoise(ofGetElapsedTimef()*noiseSpeed, i, 20) * noiseRange;
        pos.y = ofNoise(ofGetElapsedTimef()*noiseSpeed, i, 100) * noiseRange;
        float noiseScale = MIN(1,timer/growTime);
        
        ofPushMatrix();
        ofTranslate(0,generalYOffset);
        ofTranslate(pos.x*noiseScale, pos.y*noiseScale);
        ofScale(logoScale, logoScale);
        
        logoPics[i].draw(-logoPics[i].getWidth()/2, -logoPics[i].getHeight()/2);
    
        ofPopMatrix();
    }
    
    ofDisableSmoothing();
}

void Logo::drawHeadphoneMessage(){
    
    
    float prcToShrink = 0.75;
    
    float creditsY = 90;
    float headphoneY = 180;
    
    if (arcadeMode){
        creditsY = 80;
        headphoneY = 180;
    }
    
    if (usingiPad){
        creditsY = 200;
        headphoneY = 375;
    }
    
    float curHeadphoneScale = 1.2 + sin(ofGetElapsedTimef()) * 0.1;
        
    if (prcComplete < prcToShrink){
        float prc = ofMap(prcComplete, 0, prcToShrink, 0, 1);
        prc = powf(prc, 2);
        curHeadphoneScale *= prc;
    }
    
    //by line
    
    ofSetColor(0, 100*prcComplete);
    string creditsMessage = "by Andy Wallace & Dan Friel";
    ofRectangle creditsRect = font->getStringBoundingBox(creditsMessage, 0, 0);
    float creditTextW = creditsRect.width;
    font->drawString(creditsMessage, -creditTextW/2, creditsY);
    
    
    //headphone message
    
    if (!arcadeMode){
        ofPushMatrix();
        ofTranslate(0, headphoneY);
        ofScale(curHeadphoneScale, curHeadphoneScale);
        
        string headphoneMessage = "Headphones recommended";
        if (arcadeMode) headphoneMessage = "available for free at bleepspace.com";
        ofRectangle headphoneRect = font->getStringBoundingBox(headphoneMessage, 0, 0);
        float headphoneTextW = headphoneRect.width;
        float headphoneTextH = headphoneRect.height;
        ofSetColor(0, 100);
        font->drawString(headphoneMessage, -headphoneTextW/2, headphoneTextH/4);
        
        ofPopMatrix();
    }
    
    if (arcadeMode){
        
        for (int i=0; i<2; i++){
            ofSetColor(0, 100*prcComplete);
            
            ofRectangle textRect = font->getStringBoundingBox(arcadeMessage[i], 0, 0);
            float textW = textRect.width;
            font->drawString(arcadeMessage[i], -textW/2, headphoneY + 50*i);
        }
    }
}

//void Logo::drawCredits(){
//    
//    
//}
