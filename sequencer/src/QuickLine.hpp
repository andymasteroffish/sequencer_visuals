//
//  QuickLine.hpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#ifndef QuickLine_hpp
#define QuickLine_hpp

#include "ofMain.h"

class QuickLine{
public:
    
    void setup(float x1, float y1, float x2, float y2, float connectTime, float pauseTime, float disconnectTime);
    void setCurve(float connect, float disconnect);
    void update(float deltaTime);
    void draw();
    
    ofVec2f pntA, pntB;
    
    float timeToConnect;
    float timeToPause;
    float timeToDisconnect;
    
    float curveConnect, curveDisconnect;
    
    
    float timer;
    
    
    
};

#endif /* QuickLine_hpp */
