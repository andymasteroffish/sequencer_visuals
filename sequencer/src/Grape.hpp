//
//  Grape.hpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#ifndef Grape_hpp
#define Grape_hpp

#include "ofMain.h"
#include "QuickLine.hpp"

class Grape{
public:
    
    void setup(ofVec2f _pos, float delayTime);
    void setCurve(float grow, float pop, float line);
    void update(float deltaTime);
    void draw();
    
    float timer;
    
    ofVec2f pos;
    
    float growTime;
    float pauseTime;
    float popTime;
    
    float growCurve, popCurve, lineCurve;
    
    float startSize, normSize, popSize;
    float size;
                  
    
    vector<QuickLine> popLines;
    
    
};

#endif /* Grape_hpp */
