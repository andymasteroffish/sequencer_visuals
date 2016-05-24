//
//  Hit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#include "Hit.hpp"


void Hit::setup(bool _bandsOn[NUM_BANDS], int _gameW, int _gameH){
    gameW = _gameW;
    gameH = _gameH;
    
    //set full bands
    for (int i=0; i<NUM_BANDS; i++){
        bandsOn[i] = _bandsOn[i];
    }
    
    //set half bands
    for (int i=0; i<NUM_BANDS; i+=2){
        bandsOnMicro[i/2] = bandsOn[i] || bandsOn[i+1];
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

