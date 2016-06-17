//
//  BeatMarker.hpp
//  sequencer
//
//  Created by Andrew Wallace on 6/17/16.
//
//

#ifndef BeatMarker_hpp
#define BeatMarker_hpp

#include "ofMain.h"

class BeatMarker{
public:
    
    void setup(float x, float y);
    void update(float deltaTime);
    void triggerBeat();
    void triggerClear();
    void draw(bool hasSound);
    //void windowResized();
    
    ofVec2f pos;
    float curSize;
    
    float normSize;
    float pulseSize;
    
    float pulseTime;
    float pulseTimer;
    
    //growing after being cleared
    bool beingCleared;
    float clearTimer;
    float fallY, fallDist;
    
    float clearPauseRange;
    float fallTime;
    float pauseTime;
    float growTime;
    float growHighScale;
    
    float curScale;     //for growing fresh
    
};

#endif /* BeatMarker_hpp */