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
    
    if (arcadeMode){
        pos = getArcadePoint(arcadeModeDist);
    }
    
    int numGrapes = 6;//ofRandom(4,7);
    
    
    float curAngle = ofRandom(TWO_PI);
    float maxAngleChange = TWO_PI*0.2;
    
    float timeSpacing = 0.13;
    grapes.resize(numGrapes);
    
    for (int i=0; i<grapes.size(); i++){
        
        curAngle += ofRandom(-maxAngleChange, maxAngleChange);
        float dist = ofRandom(25,40);//ofRandom(30,60);
        
        pos.x += cos(curAngle) * dist;
        pos.y += sin(curAngle) * dist;
        
        
        grapes[i].setup(pos, timeSpacing*i);
    }
    
}

void GrapesHit::updateCustom(){
    for (int i=0; i<grapes.size(); i++){
        grapes[i].update(deltaTime);
    }
    
    int last = grapes.size()-1;
    if (grapes[last].popLines[last].timer > grapes[last].popLines[last].timeToDisconnect){
        killMe = true;
    }
}

void GrapesHit::draw(){
    setLineWidth(1);
    for (int i=0; i<grapes.size(); i++){
        grapes[i].draw();
    }
}
