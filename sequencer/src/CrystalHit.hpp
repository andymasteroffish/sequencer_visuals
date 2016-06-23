//
//  CrystalHit.hpp
//  sequencer
//
//  Created by Andrew Wallace on 6/8/16.
//
//

#ifndef CrystalHit_hpp
#define CrystalHit_hpp

#include "Hit.hpp"
#include "QuickLine.hpp"

class CrystalHit : public Hit{
public:
    
    void setupCustom();
    void updateCustom();
    void draw();
    
    QuickLine lines[6];
    
    int numPopLines;
    QuickLine popLines[5];
    
    
    ofVec2f pos;
    
    float angle;
    float startAngle, endAngle;
    
};

#endif /* CrystalHit_hpp */
