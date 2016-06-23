//
//  Hit.hpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#ifndef Hit_hpp
#define Hit_hpp

#include "ofMain.h"

#define NUM_BANDS 8

class Hit{
public:
    
    void setup(int _gameW, int _gameH, int _whiteVal);
    virtual void setupCustom(){}
    void update(float _deltaTime);
    virtual void updateCustom(){}
    
    virtual void draw(){}
    
    virtual void cleanUp(){}
    
    float deltaTime;
    
    int gameW, gameH;
    int whiteVal;
    
    bool killMe;
    
    float timer;
    
    //for messing with visuals
    float zVal;
    
    
};

#endif /* Hit_hpp */
