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
    
    void setup(int _gameW, int _gameH, int _whiteVal, bool _usingiPad, bool usingArcade, float _arcadeScale);
    virtual void setupCustom(){}
    void update(float _deltaTime);
    virtual void updateCustom(){}
    
    virtual void draw(){}
    
    //virtual void cleanUp(){}
    
    ofVec2f getArcadePoint(float maxDist);
    
    void setLineWidth(float val);
    
    float deltaTime;
    
    int gameW, gameH;
    int whiteVal;
    
    bool killMe;
    
    float timer;
    
    bool usingiPad;
    
    //for messing with visuals
    float zVal;
    
    //float lineWidthPrc;
    
    
    //arcade shit
    bool arcadeMode;
    float arcadeScale;
    float arcadeModeDist;
    
    //testing
    int idNum;
    
};

#endif /* Hit_hpp */
