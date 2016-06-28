//
//  WaveColumnHit.hpp
//  sequencer
//
//  Created by Andrew Wallace on 6/8/16.
//
//

#ifndef WaveColumnHit_hpp
#define WaveColumnHit_hpp

#include "Hit.hpp"

class WaveColumnHit : public Hit{
    
    void setupCustom();
    void updateCustom();
    void draw();
    
    ofVec2f pos;
    
    float growTime;
    float waveStartTime;
    float pauseTime;
    float shrinkTime;
    
    int numPoints;
    vector<ofVec2f> pnts;
    vector<ofVec2f> startPnts;
    vector<ofVec2f> basePnts;
    vector<ofVec2f> endPoints;
    
    float waveSpeed;
    float waveZoom;
    float fullWaveAmount;
    
    float angle;
    
};

#endif /* WaveColumnHit_hpp */
