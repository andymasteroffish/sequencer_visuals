//
//  DemoHit.hpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#ifndef DemoHit_hpp
#define DemoHit_hpp

#include "Hit.hpp"

class DemoHit : public Hit{
public:
    
    void setupCustom();
    void updateCustom();
    void draw();
    
    
    ofVec2f pos;
    float riseSpeed;
    
};

#endif /* DemoHit_hpp */
