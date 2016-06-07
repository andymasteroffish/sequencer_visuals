//
//  DrunkTriangleHit.hpp
//  sequencer
//
//  Created by Andrew Wallace on 6/7/16.
//
//

#ifndef DrunkTriangleHit_hpp
#define DrunkTriangleHit_hpp

#include "Hit.hpp"

class DrunkTriangleHit : public Hit{
public:
    
    void setupCustom();
    void updateCustom();
    void draw();
    
    int numSteps;
    
    float stepDist;
    
    float growTime;
    float normalizeTime;
    float pauseTime;
    float growBeforeDeathTime;
    float shrinkTime;
    
    float timerOffset;
    
    float normalSize;
    float largeSize;
    
    ofVec2f startPos;
    vector<float> angles;
    vector<float> sizes;
    
};

#endif /* DrunkTriangleHit_hpp */
