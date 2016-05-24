//
//  SweepHit.hpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#ifndef SweepHit_hpp
#define SweepHit_hpp

#include "Hit.hpp"

class SweepHit : public Hit{
public:
    
    void setupCustom();
    void updateCustom();
    void draw();
    
    int dir;
    float xPos;
    
    float startX, endX;
    
    float sweepTime;
    //float speed;
    
    int trailLength;
    
};

#endif /* SweepHit_hpp */
