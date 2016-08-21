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
    
    void setup(float x, float _stepModeX, float y, bool usingIpad);
    void update(float deltaTime, bool stepModeOn, float firstRunTimer);
    void triggerBeat();
    void triggerClear();
    void draw(bool hasSound, bool _isRecording);
    
    ofVec2f pos;
    
    float normX, stepModeX;
    float xenoX;
    
    float curSize;
    
    float normSize;
    float pulseSize;
    
    float pulseTime;
    float pulseTimer;
    
    //growing after being cleared
    bool beingCleared;
    float clearTimer;
    float fallY, fallDist;
    
    float firstRunYAdjust;
    
    float notRecordingYMaxDist;
    float notRecordingYOffset;
    
    float clearPauseRange;
    float fallTime;
    float pauseTime;
    float growTime;
    float growHighScale;
    
    float curScale;     //for growing fresh
    
    bool isRecording;
    
};

#endif /* BeatMarker_hpp */
