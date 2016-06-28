//
//  BuckshotHit.hpp
//  sequencer
//
//  Created by Andrew Wallace on 5/25/16.
//
//

#ifndef BuckshotHit_hpp
#define BuckshotHit_hpp

#include "Hit.hpp"
#include "Buckshot.hpp"

class BuckshotHit : public Hit{
public:
    
    void setupCustom();
    void updateCustom();
    void draw();
    
    
    vector<Buckshot> shots;
    
    
};

#endif /* BuckshotHit_hpp */
