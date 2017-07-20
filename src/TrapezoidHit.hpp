//
//  TrapezoidHit.hpp
//  sequencer
//
//  Created by Andrew Wallace on 5/25/16.
//
//

#ifndef TrapezoidHit_hpp
#define TrapezoidHit_hpp

#include "Hit.hpp"
#include "QuickLine.hpp"

class TrapezoidHit : public Hit{
public:
    
    void setupCustom();
    void updateCustom();
    void draw();
    
    ofVec2f pos;
    float drawAngle;
    
    float topY, bottomY;
    float topW, bottomW;
    
    float startTopW, startBotW;
    
    float height;
    
    float fallTime;
    
    int phase;
    
    
    vector<QuickLine> lines;
    
    
};

#endif /* TrapezoidHit_hpp */
