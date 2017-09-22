//
//  SpokesHit.hpp
//  sequencer
//
//  Created by Andrew Wallace on 9/22/17.
//
//

#ifndef SpokesHit_hpp
#define SpokesHit_hpp

#include "ofMain.h"
#include "Hit.hpp"
#include "Spoke.hpp"

class SpokesHit : public Hit{
    
    void setupCustom();
    void updateCustom();
    void draw();
    
    ofVec2f pos;
    
    float shrinkTime;
    float rotTime;
    float killTime;
    
    vector<Spoke> spokes;
    
};

#endif /* SpokesHit_hpp */
