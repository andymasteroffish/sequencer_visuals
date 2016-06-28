//
//  SquareHit.hpp
//  sequencer
//
//  Created by Andrew Wallace on 5/25/16.
//
//

#ifndef SquareHit_hpp
#define SquareHit_hpp

#include "Hit.hpp"
#include "QuickLine.hpp"

class SquareHit : public Hit{
public:
    
    void setupCustom();
    void updateCustom();
    void draw();
    
    ofVec2f pos;
    
    QuickLine lines[4]; //top, left, botom, right
    
    float startFallTime;
    
    float fallTime;
    float angle;
    float angleOvershoot;
    float angleOvershootTime;
    float angleEnd;
    
};

#endif /* SquareHit_hpp */
