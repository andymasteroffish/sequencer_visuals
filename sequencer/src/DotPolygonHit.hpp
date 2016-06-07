//
//  DotPolygonHit.hpp
//  sequencer
//
//  Created by Andrew Wallace on 6/7/16.
//
//

#ifndef DotPolygonHit_hpp
#define DotPolygonHit_hpp

#include "Hit.hpp"
#include "QuickLine.hpp"

class DotPolygonHit : public Hit{
public:
    
    void setupCustom();
    void updateCustom();
    void draw();
    
    int numPoints;
    
    float growTime;
    float normalizeTime;
    float preDeathGrowTime;
    float shrinkTime;
    
    vector<float> startDeathTimes;
    
    vector<ofVec2f> points;
    vector<float> pointSize;
    
    vector<QuickLine> lines;
    
    float startSize;
    float largeSize;
    float normalSize;
    
    
    
    
    
    
};

#endif /* DotPolygonHit_hpp */
