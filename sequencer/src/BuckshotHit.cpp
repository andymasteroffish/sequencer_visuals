//
//  BuckshotHit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 5/25/16.
//
//

#include "BuckshotHit.hpp"


void BuckshotHit::setupCustom(){
    
    int numShots = ofRandom(9,15);
    
    float range = 90;//60;
    float padding = 100;
    
    float centerX = ofRandom(padding, gameW-padding);
    float centerY = ofRandom(padding, gameH-padding);
    
    shots.resize(numShots);
    for (int i=0; i<numShots; i++){
        float angle = ofRandom(TWO_PI);
        float dist = ofRandom(range);
        float thisX = centerX + cos(angle) * dist;
        float thisY = centerY + sin(angle) * dist;
        shots[i].setup(thisX, thisY, ofRandom(0,0.1));
    }
    
    
    
}

void BuckshotHit::updateCustom(){
    killMe = true;
    for (int i=0; i<shots.size(); i++){
        shots[i].update(deltaTime);
        if (!shots[i].isDone){
            killMe = false;
        }
    }
}

void BuckshotHit::draw(){
    for (int i=0; i<shots.size(); i++){
        shots[i].draw();
    }
}