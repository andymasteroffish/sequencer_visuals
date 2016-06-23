//
//  Hit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#include "Hit.hpp"


void Hit::setup(int _gameW, int _gameH, int _whiteVal){
    gameW = _gameW;
    gameH = _gameH;
    
    whiteVal = _whiteVal;
    
    killMe = false;
    
    timer = 0;
    
    zVal = ofRandom(-400, 200);
    
    setupCustom();
}

void Hit::update(float _deltaTime){
    deltaTime = _deltaTime;
    
    timer += deltaTime;
    
    updateCustom();
}

