//
//  Chaser.hpp
//  sequencer
//
//  Created by Andrew Wallace on 5/25/16.
//
//

#ifndef Chaser_hpp
#define Chaser_hpp

#include "Hit.hpp"

class ChaserHit : public Hit{
public:
    
    void setupCustom();
    void updateCustom();
    void draw();
    
    ofVec2f pos;
    
    int numPoints;
    vector<ofVec2f> points;
    
    float fillTime;
    
    int phase;
    
    float angle;    //degrees
    
};

#endif /* Chaser_hpp */
