//
//  TunnelHit.hpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#ifndef TunnelHit_hpp
#define TunnelHit_hpp

#include "Hit.hpp"

class TunnelHit : public Hit{
public:
    
    void setupCustom();
    void updateCustom();
    void draw();
    
    ofVec2f pos;
    float size;
    //float growthRate;
    
    float startSize;
    float smallSize;
    float bigSize;
    
    float shrinkTime, growTime;
    
};

#endif /* TunnelHit_hpp */
