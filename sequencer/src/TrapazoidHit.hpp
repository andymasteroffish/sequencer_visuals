//
//  TrapazoidHit.hpp
//  sequencer
//
//  Created by Andrew Wallace on 5/25/16.
//
//

#ifndef TrapazoidHit_hpp
#define TrapazoidHit_hpp

#include "Hit.hpp"
#include "QuickLine.hpp"

class TrapazoidHit : public Hit{
public:
    
    void setupCustom();
    void updateCustom();
    void draw();
    
    ofVec2f pos;
    
    float topY, bottomY;
    float topW, bottomW;
    
    float startTopW, startBotW;
    
    float height;
    
    float fallTime;
    
    int phase;
    
    
    vector<QuickLine> lines;
    
    
};

#endif /* TrapazoidHit_hpp */
