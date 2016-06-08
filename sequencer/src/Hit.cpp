//
//  Hit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#include "Hit.hpp"


void Hit::setup(bool _bandsOnMicro[NUM_BANDS/2], int _gameW, int _gameH, int _whiteVal){
    gameW = _gameW;
    gameH = _gameH;
    
    whiteVal = _whiteVal;
    
//    //set full bands
//    for (int i=0; i<NUM_BANDS; i++){
//        bandsOn[i] = _bandsOn[i];
//    }
    
    //set half bands
    for (int i=0; i<NUM_BANDS/2; i+=2){
        bandsOnMicro[i] = _bandsOnMicro[i];
    }
    
    killMe = false;
    
    timer = 0;
    
    setupCustom();
}

void Hit::update(float _deltaTime){
    deltaTime = _deltaTime;
    
    timer += deltaTime;
    
    updateCustom();
}

