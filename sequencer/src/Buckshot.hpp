//
//  Buckshot.hpp
//  sequencer
//
//  Created by Andrew Wallace on 5/25/16.
//
//

#ifndef Buckshot_hpp
#define Buckshot_hpp

#include "ofMain.h"

class Buckshot{
public:
    
    void setup(float x, float y, float delayTime);
    void update(float deltaTime);
    void draw();
    
    
    ofVec2f pos;
    float size;
    
    float timer;
    
    float startTime, pauseTime, popTime;
    float startSize, normSize, popSize;
    
    bool isDone;
    
};

#endif /* Buckshot_hpp */
