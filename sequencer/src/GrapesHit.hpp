//
//  GrapesHit.hpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#ifndef GrapesHit_hpp
#define GrapesHit_hpp

#include "Hit.hpp"
#include "Grape.hpp"

class GrapesHit : public Hit{
public:
    
    void setupCustom();
    void updateCustom();
    void draw();
    
    
    vector<Grape> grapes;
    
    
};

#endif /* GrapesHit_hpp */
