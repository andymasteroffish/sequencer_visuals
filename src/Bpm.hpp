//
//  Bpm.hpp
//  sequncer
//
//  Created by Andrew Wallace on 7/5/16.
//
//

//This class is heavily based on
//ofxBPM tools by mirrorboy714
//https://github.com/mirrorboy714/ofxBpm

#ifndef Bpm_hpp
#define Bpm_hpp

#include "ofMain.h"
#include "ofThread.h"

class Bpm : private ofThread{
    
public:
    
    void start(float bmpValue);
    void stop();
    void setPreHitPrcSpacing(float prc);
    void setBpm(float newBpm);
    
    
    void threadedFunction();
    
    ofEvent<void> beatEvent;
    ofEvent<void> preBeatEvent;
    
protected:
    
    int millisBetweenBeats;
    int nextBeatTime;
    
    float preHitPrc;
    int preHitMilliRange;
    bool hasDonePreHit;
    
    
    
    
};

#endif /* Bpm_hpp */
