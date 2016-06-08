//
//  CrossBoxHit.hpp
//  sequencer
//
//  Created by Andrew Wallace on 6/8/16.
//
//

#ifndef CrossBoxHit_hpp
#define CrossBoxHit_hpp

#include "Hit.hpp"
#include "QuickLine.hpp"

class CrossBoxHit : public Hit{
public:
    
    void setupCustom();
    void updateCustom();
    void draw();
    
    QuickLine lines[8];
    
    ofVec2f pos;
    
    float angle;
    float startAngle, endAngle;
    
};

#endif /* CrossBoxHit_hpp */
