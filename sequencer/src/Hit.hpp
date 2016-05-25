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
    
    void setup(bool _bandsOnMicro[NUM_BANDS/2], int _gameW, int _gameH);
    virtual void setupCustom(){}
    void update(float _deltaTime);
    virtual void updateCustom(){}
    
    virtual void draw(){}
    
    virtual void cleanUp(){}
    
    float deltaTime;
    
    int gameW, gameH;
    
    //bool bandsOn[NUM_BANDS];
    bool bandsOnMicro[NUM_BANDS/2];
    
    
    bool killMe;
    
    float timer;
    
    
};

#endif /* Hit_hpp */
