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
    
    void setup(float x, float _stepModeX, float y, int _whiteVal, bool usingIpad);
    void arcadeSetPos(float prc, float beatMarkerPrcDist);
    void update(float deltaTime, bool stepModeOn, float firstRunTimer);
    bool checkHit(int x, int y);
    void triggerBeat();
    void triggerClear();
    void draw(bool hasSound, bool hasOtherStepModeSound, bool _isRecording);
    
    ofVec2f pos;
    
    float normX, stepModeX;
    float xenoX;
    
    float curSize;
    
    float normSize;
    float pulseSize;
    
    float pulseTime;
    float pulseTimer;
    
    bool isOnTheOne;
    
    //growing after being cleared
    bool beingCleared;
    bool hadSoundOnClear, hadOtherStepModeSoundOnClear;
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
    
    int whiteVal;
    
    bool isRecording;
    
    //tapping
    float maxTapDist;
    
    //for arcade mode
    bool arcadeMode;
    float arcadeAngle;
    
};

#endif /* BeatMarker_hpp */
