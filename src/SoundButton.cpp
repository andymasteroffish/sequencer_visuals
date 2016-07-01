//
//  SoundButton.cpp
//  sequncer
//
//  Created by Andrew Wallace on 7/1/16.
//
//

#include "SoundButton.hpp"

void SoundButton::setup(float x, float y, float w, float h, int _idNum){
    
    hitBox.set(x, y, w, h);
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
    
    
}

void SoundButton::update(float deltaTime, int curSound){
    isActive = curSound == idNum;
    float targetAlpha = 80;
    if (isActive){
        targetAlpha = 255;
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
    ofSetLineWidth(1);
    
    ofPushMatrix();
    ofTranslate(hitBox.x+hitBox.width/2, hitBox.y+hitBox.height/2);
    
    ofScale(curScale*bounceScale, curScale*bounceScale);
    
    icons.draw(idNum, curAlpha);
    
    ofPopMatrix();
    
    
//    ofSetLineWidth(1);
//    ofNoFill();
//    ofSetColor(0);
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
    growTimer = -ofRandom(0.15);
}

void SoundButton::stepModeOff(){
    isShrinking = true;
    isGrowing = false;
    growTimer = shrinkTime + ofRandom(0.15);
    
}