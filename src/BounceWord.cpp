//
//  BounceWord.cpp
//  sequencer
//
//  Created by Andrew Wallace on 7/9/16.
//
//

#include "BounceWord.hpp"


void BounceWord::setup(string _text, float x, float y, ofTrueTypeFont * _font){
    text = _text;
    pos.set(x, y);
    
    font = _font;
    
    textW = font->stringWidth(text);
    textH = font->getLineHeight();
    
    //modify the pos to compensate for centerring the text
    pos.x += textW/2;
    pos.y -= textH/2;
    
    growTime = 0.2;
    shrinkXeno = 0.95;
    largeScale = 1.2;
    
    reset();
    
}

void BounceWord::reset(){
    timer = -0.2 - ofRandom(0.3);
    isGrowing = true;
}
void BounceWord::close(){
    timer = growTime + 0.1 + ofRandom(0.15);
    isGrowing = false;
}

void BounceWord::update(float deltaTime){
    if (isGrowing){
        timer += deltaTime;
    }else{
        timer -= deltaTime;
    }
    
    if (timer < 0){
        curScale = 0;
    }
    else if (timer > 0 && timer <= growTime){
        float prc = timer/growTime;
        prc = powf(prc, 0.7);
        curScale = prc * largeScale;
    }
    else if (timer > growTime){
        if (isGrowing){
            curScale = shrinkXeno * curScale + (1-shrinkXeno) * 1;
        }else{
            curScale = shrinkXeno * curScale + (1-shrinkXeno) * largeScale;
        }
    }
    
    
    //testing
    //curScale = 1;//ofMap( sin(ofGetElapsedTimef() + pos.x*0.1), -1, 1, 0.9, 1.1);
}

void BounceWord::draw(){
    
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofScale(curScale, curScale);
    font->drawString(text, -textW/2, textH/2);
    
    ofPopMatrix();
    
}