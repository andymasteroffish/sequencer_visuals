//
//  Bpm2.hpp
//  bleepspace
//
//  Created by Andrew Wallace on 6/5/17.
//
//

#ifndef Bpm2_hpp
#define Bpm2_hpp

#include "ofMain.h"
#include "ofThread.h"

class Bpm2: public ofThread{
    
public:
    
    void start(float bmpValue);
    void stop();
    void setPreHitPrcSpacing(float prc);
    void setBpm(float newBpm);
    void justGotFocus();

    ofEvent<void> beatEvent;
    ofEvent<void> preBeatEvent;
    
private:
    ofTimer timer;
    void threadedFunction();
    
    
    int millisBetweenBeats;
    int nextBeatTime;
    
    float preHitPrc;
    int preHitMilliRange;
    bool hasDonePreHit;
    
    
};

#endif /* Bpm2_hpp */
