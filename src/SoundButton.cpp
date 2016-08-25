//
//  SoundButton.cpp
//  sequncer
//
//  Created by Andrew Wallace on 7/1/16.
//
//

#include "SoundButton.hpp"

void SoundButton::setup(int _idNum, int whiteVal){
    idNum = _idNum;
    
    onScale = 1;
    largeScale = onScale * 1.5;
    
    growTime = 0.3;
    shrinkTime = growTime + 0.1;
    growTimer = 0;
    isGrowing = false;
    isShrinking = false;
    
    bounceGrowTime = 0.2;
    bounceShrinkTime = bounceGrowTime + 0.3;
    bounceTimer = bounceShrinkTime;
    
    curAlpha = 0;
    
    icons.setup();
    icons.whiteVal = whiteVal;
    
}

void SoundButton::setPosition(float x, float y, float w, float h){
    hitBox.set(x, y, w, h);
}

void SoundButton::update(float deltaTime, int curSound, bool beatIsUsed){
    isActive = curSound == idNum;
    float targetAlpha = 100;
    if (isActive){
        targetAlpha = 255;
    }
    if (isActive || beatIsUsed){
        icons.update(deltaTime);
    }
    
    float fadeXeno = 0.9;
    curAlpha = fadeXeno * curAlpha + (1-fadeXeno) * targetAlpha;
    
    //growing or shrinking
    curScale = 0;
    if (isGrowing)      growTimer += deltaTime;
    if (isShrinking)    growTimer -= deltaTime;
    
    if (growTimer < 0){
        curScale = 0;
    }
    else if (growTimer > 0 && growTimer <= growTime){
        float prc = growTimer/growTime;
        prc = powf(prc, 0.6);
        curScale = prc * largeScale;
    }
    else if (growTimer > growTime && growTimer <= shrinkTime){
        float prc = (growTimer-growTime)/(shrinkTime-growTime);
        prc = powf(prc, 1.2);
        curScale = (1-prc) * largeScale + prc * onScale;
    }
    else if (growTimer > shrinkTime){
        curScale = onScale;
    }
    
    //bouncing
    bounceTimer += deltaTime;
    bounceScale = 1;
    if (bounceTimer < bounceGrowTime){
        float prc = bounceTimer/bounceGrowTime;
        prc = powf(prc, 0.75);
        bounceScale = (1-prc) * 1 + prc * largeScale;
    }
    else if (bounceTimer >= bounceGrowTime && bounceTimer < bounceShrinkTime){
        float prc = (bounceTimer-bounceGrowTime)/(bounceShrinkTime-bounceGrowTime);
        prc = powf(prc, 1.5);
        bounceScale = (1-prc) * largeScale + prc * 1;
    }
    
    //cout<<"sc "<<curScale<<"  bs "<<bounceScale<<endl;
    
}

void SoundButton::draw(){
    
    if (curScale > 0){
        ofFill();
        float fillAlpha =  170 * curScale;
        ofSetColor(icons.whiteVal, fillAlpha);
        ofDrawRectangle(hitBox);
        
        ofSetLineWidth(1);
        ofNoFill();
        ofSetColor(0, curAlpha * curScale);
        ofDrawRectangle(hitBox);
    }
    
    ofSetLineWidth(1);
    
    ofPushMatrix();
    ofTranslate(hitBox.x+hitBox.width/2, hitBox.y+hitBox.height/2);
    
    ofScale(curScale*bounceScale, curScale*bounceScale);
    
    float linePrc =  isActive ? 4 : 1;
    icons.draw(idNum, linePrc, curAlpha);
    
    ofPopMatrix();
    
//    ofNoFill();
//    ofSetColor(255,0,0);
//    ofDrawRectangle(hitBox);
    
   
    
    
    
}

bool SoundButton::checkHit(int x, int y){
    
    if (hitBox.inside(x, y)){
        bounceTimer = 0;
        return true;
    }
    
    return false;
}

void SoundButton::stepModeOn(){
    isGrowing = true;
    isShrinking = false;
    growTimer = -abs( 7-idNum) * 0.04;//  -idNum * 0.03;// -ofRandom(0.15);
}

void SoundButton::stepModeOff(){
    isShrinking = true;
    isGrowing = false;
    growTimer = shrinkTime + abs( 7-idNum) * 0.04;//ofRandom(0.15);
    
}