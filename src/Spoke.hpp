//
//  Spoke.hpp
//  sequencer
//
//  Created by Andrew Wallace on 9/22/17.
//
//

#ifndef Spoke_hpp
#define Spoke_hpp

#include "ofMain.h"

class Spoke{
public:
    
    void setup(float _targetDist, float _angle);
    void setTargetDist(float _targetDist);
    
    void update(float time);
    void fixedUpdate();
    void drawLine();
    void drawCircle();
    
    ofVec2f anchorPos;
    float angle;
    
    float curDist;
    
    float frc;
    float pullForce;
    float curVel;
    float targetDist;
    
    float timeBetweenUpdates;
    float nextUpdateTime;
    
    float circleSize;
    
};

#endif /* Spoke_hpp */
