//
//  SlashHit.hpp
//  sequencer
//
//  Created by Andrew Wallace on 5/25/16.
//
//

#ifndef SlashHit_hpp
#define SlashHit_hpp

#include "Hit.hpp"
#include "QuickLine.hpp"

class SlashHit : public Hit{
public:
    
    void setupCustom();
    void updateCustom();
    void draw();
    
    ofVec2f pos;
    
    QuickLine lineA, lineB;
    
    
    float circleSize;
    float circleBigSize, circleNormSize;
    float circleStartGrowTime, circleGrowTime, circlePauseTime, circleShrinkTime;
    
    
};

#endif /* SlashHit_hpp */
