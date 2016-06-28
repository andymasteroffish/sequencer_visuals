//
//  ClapHit.hpp
//  sequencer
//
//  Created by Andrew Wallace on 6/8/16.
//
//

#ifndef ClapHit_hpp
#define ClapHit_hpp

#include "Hit.hpp"
#include "QuickLine.hpp"

class ClapHit : public Hit{
public:
    
    void setupCustom();
    void updateCustom();
    void draw();
    
    ofVec2f pos;
    
    ofVec2f pntA, pntB;
    float pntStartDist;  //this could be different for x and Y
    float pntDist;
    
    float pntAngle;
    float startAngle;
    
    float firstClapTime;
    float secondClapTime;
    float pauseTime;
    float recedeTime;
    
    float curScale;
    float startScale;
    float impactTime;
    
    float popCircleTime;
    float popCircleSize;
    float popCircleDist;
    float leftPopCircleSize;
    float rightPopCircleSize;
    
    float angle;
    
};

#endif /* ClapHit_hpp */
