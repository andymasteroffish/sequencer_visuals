//
//  Hit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#include "Hit.hpp"


void Hit::setup(int _gameW, int _gameH, int _whiteVal, bool usingiPad, bool _arcadeMode){
    gameW = _gameW;
    gameH = _gameH;
    
    whiteVal = _whiteVal;
    
    lineWidthPrc = usingiPad ? 2 : 1;
    
    killMe = false;
    
    timer = 0;
    
    zVal = ofRandom(-400, 200);
    
    arcadeMode = _arcadeMode;
    arcadeModeDist = ofGetHeight() * 0.4;
    
    setupCustom();
}

void Hit::update(float _deltaTime){
    deltaTime = _deltaTime;
    
    timer += deltaTime;
    
    updateCustom();
}


ofVec2f Hit::getArcadePoint(float maxDist){
    ofVec2f val;
    float thisDist = ofRandom(0,maxDist);
    float thisAngle = ofRandom(0,TWO_PI);
    val.x = ofGetWidth()/2 + cos(thisAngle) * thisDist;
    val.y = ofGetHeight()/2 + sin(thisAngle) * thisDist;
    return val;
}
