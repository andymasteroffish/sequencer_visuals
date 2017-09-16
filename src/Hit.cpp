//
//  Hit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#include "Hit.hpp"


void Hit::setup(int _gameW, int _gameH, int _whiteVal, bool _usingiPad, bool usingArcade, float _arcadeScale){
    gameW = _gameW;
    gameH = _gameH;
    
    whiteVal = _whiteVal;
    
    usingiPad = _usingiPad;
    
    //lineWidthPrc = usingiPad ? 2 : 1;
    
    killMe = false;
    
    timer = 0;
    
    zVal = ofRandom(-400, 200);
    
    arcadeMode = usingArcade;
    arcadeScale = _arcadeScale;
    arcadeModeDist = (ofGetHeight() * 0.4) / arcadeScale;
    
    setupCustom();
}


void Hit::update(float _deltaTime){
    //cout<<"update hit "<<idNum<<endl;
    deltaTime = _deltaTime;
    
    timer += deltaTime;
    
    updateCustom();
}

void Hit::drawTest(){
    cout<<"draw hit "<<idNum<<endl;
    draw();
}

ofVec2f Hit::getArcadePoint(float maxDist){
    ofVec2f val;
    float thisDist = ofRandom(0,maxDist);
    float thisAngle = ofRandom(0,TWO_PI);
    val.x = ofGetWidth()/2 + cos(thisAngle) * thisDist;
    val.y = ofGetHeight()/2 + sin(thisAngle) * thisDist;
    return val;
}

void Hit::setLineWidth(float val){
    float newVal = val;
    if (!arcadeMode){
        if (usingiPad){
            newVal = val * 2;
        }
    }else{
       newVal = val * arcadeScale;
    }
    
    //cout<<"was "<<val<<" is "<<newVal<<endl;
    
    ofSetLineWidth(newVal);
}
