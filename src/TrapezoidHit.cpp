//
//  TrapezoidHit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 5/25/16.
//
//

#include "TrapezoidHit.hpp"


void TrapezoidHit::setupCustom(){
    
    topW = 60;
    bottomW = 40;
    
    startTopW = topW;
    startBotW = bottomW;
    
    topY = 0;
    bottomY = 0;
    
    height = 50;
    
    pos.x = ofRandom(bottomW, gameW-bottomW);
    pos.y = ofRandom(0, gameH-height*2);
    
    if (ofRandomuf() > 0.5){
        pos.y = ofRandom(height*2, gameH);
        height *= -1;
    }
    
    fallTime = 0.23;//0.39;
    
    phase = 0;
    
    //setup some lines
    float lineYOffset = 10;
    float lineNearXOffset = 10;
    float lineFarXOffset = 30;
    
    float lineGrowTime = 0.15;
    float linePauseTime = 0.15;
    float lineShrinkTime = 0.15;
    
    lines.resize(4);
    
    //left side
    lines[0].setup(-bottomW-lineNearXOffset, height, -bottomW-lineFarXOffset, height - lineYOffset, lineGrowTime, linePauseTime, lineShrinkTime);
    lines[1].setup(-bottomW-lineNearXOffset, height, -bottomW-lineFarXOffset, height + lineYOffset, lineGrowTime, linePauseTime, lineShrinkTime);
    //right side
    lines[2].setup(bottomW+lineNearXOffset, height, bottomW+lineFarXOffset, height - lineYOffset, lineGrowTime, linePauseTime, lineShrinkTime);
    lines[3].setup(bottomW+lineNearXOffset, height, bottomW+lineFarXOffset, height + lineYOffset, lineGrowTime, linePauseTime, lineShrinkTime);
    
    for (int i=0; i<lines.size(); i++){
        lines[i].timer = -fallTime;
        lines[i].setCurve(0.5, 1);
    }
    
}

void TrapezoidHit::updateCustom(){
    
    if (timer > fallTime){
        timer = fallTime;
    }
    
    
    if (phase == 0){
        float prc = timer/fallTime;
        prc = powf(prc, 2);
        bottomY = prc * height;
    }
    
    if (phase == 1){
        float prc = timer/fallTime;
        prc = powf(prc, 2);
        topY = prc * height*2;
        bottomY = height;
    }
    
    if (phase == 2){
        float prc = timer/fallTime;
        prc = powf(prc, 2);
        bottomY = prc * height*2 + (1-prc)*height;
        topY = height * 2;
    }
    
    if (phase == 3){
        float prc = timer/fallTime;
        prc = powf(prc, 0.5);
        //bottomY = height*2 ;
        //topY = height*2;
        topW = (1-prc) * startTopW;
        bottomW = (1-prc) * startBotW;
    }
    
    if (timer >= fallTime){
        timer = 0;
        phase++;
    }
    
    if (phase == 4){
        killMe = true;
    }
    
    for (int i=0; i<lines.size(); i++){
        lines[i].update(deltaTime);
    }
    
}

void TrapezoidHit::draw(){
    
    ofSetColor(0);
    ofSetLineWidth(2 * lineWidthPrc);
    
    ofPushMatrix();
    
    ofTranslate(pos.x, pos.y);
    
    ofVec2f topLeft(-topW, topY);
    ofVec2f topRight(topW, topY);
    ofVec2f botLeft(-bottomW, bottomY);
    ofVec2f botRight(bottomW, bottomY);
    
    ofDrawLine(topLeft, topRight);
    ofDrawLine(topRight, botRight);
    ofDrawLine(botRight, botLeft);
    ofDrawLine(botLeft, topLeft);
    
    ofSetLineWidth(1 * lineWidthPrc);
    for (int i=0; i<lines.size(); i++){
        lines[i].draw();
    }
    
    ofPopMatrix();
    
}