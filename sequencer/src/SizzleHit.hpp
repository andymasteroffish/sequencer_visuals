//
//  SizzleHit.hpp
//  sequencer
//
//  Created by Andrew Wallace on 6/7/16.
//
//

#ifndef SizzleHit_hpp
#define SizzleHit_hpp

#include "Hit.hpp"

class SizzleHit : public Hit{
    
    void setupCustom();
    void updateCustom();
    void draw();
    
    
    int numPoints;
    
    ofVec2f pos;
    
    vector<ofVec2f> points;
    vector<ofVec2f> targetPoints;
    vector<ofVec2f> basePoints;
    
    float spreadTime;
    float pauseTime;
    float dissipateTime;
    
    
};

#endif /* SizzleHit_hpp */
