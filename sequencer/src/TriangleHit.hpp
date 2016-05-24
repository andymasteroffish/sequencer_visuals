//
//  TriangleHit.hpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#ifndef TriangleHit_hpp
#define TriangleHit_hpp

#include "Hit.hpp"
#include "QuickLine.hpp"

class TriangleHit : public Hit{
public:
    
    void setupCustom();
    void updateCustom();
    void draw();
    
    vector<QuickLine> lines;
    
    ofVec2f centerPos;
    
    //degrees
    float angle;
    float startAngle;
    float endAngle;
    
    //scaling it up
    float startScale, endScale;
    float scale;
};



#endif /* TriangleHit_hpp */
