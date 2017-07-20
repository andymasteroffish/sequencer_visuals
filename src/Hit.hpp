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
    
    void setup(int _gameW, int _gameH, int _whiteVal, bool usingiPad, bool _arcadeMode);
    virtual void setupCustom(){}
    void update(float _deltaTime);
    virtual void updateCustom(){}
    
    virtual void draw(){}
    
    virtual void cleanUp(){}
    
    ofVec2f getArcadePoint(float maxDist);
    
    float deltaTime;
    
    int gameW, gameH;
    int whiteVal;
    
    bool killMe;
    
    float timer;
    
    //for messing with visuals
    float zVal;
    
    float lineWidthPrc;
    
    
    //arcade shit
    bool arcadeMode;
    float arcadeModeDist;
    
    
};

#endif /* Hit_hpp */
