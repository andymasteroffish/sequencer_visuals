//
//  GrapesHit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#include "GrapesHit.hpp"


void GrapesHit::setupCustom(){
    
    float padding = 100;
    ofVec2f pos;
    pos.x = ofRandom(padding, gameW-padding);
    pos.y = ofRandom(padding, gameH-padding);
    
    int numGrapes = ofRandom(4,7);
    
    
    float curAngle = ofRandom(TWO_PI);
    float maxAngleChange = TWO_PI*0.25;
    
    float timeSpacing = 0.08;
    grapes.resize(numGrapes);
    
    for (int i=0; i<grapes.size(); i++){
        
        curAngle += ofRandom(-maxAngleChange, maxAngleChange);
        float dist = ofRandom(30,60);
        
        pos.x += cos(curAngle) * dist;
        pos.y += sin(curAngle) * dist;
        
        
        grapes[i].setup(pos, timeSpacing*i, lineWidthPrc);
    }
    
}

void GrapesHit::updateCustom(){
    for (int i=0; i<grapes.size(); i++){
        grapes[i].update(deltaTime);
    }
    
    if (grapes[0].popLines[0].timer > grapes[0].popLines[0].timeToDisconnect){
        killMe = true;
    }
}

void GrapesHit::draw(){
    for (int i=0; i<grapes.size(); i++){
        grapes[i].draw();
    }
}