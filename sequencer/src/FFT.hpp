//
//  FFT.hpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#ifndef FFT_hpp
#define FFT_hpp

#include "ofMain.h"
#include "ofxFFTLive.h"
#include "ofxFFTFile.h"
#include "Hit.hpp"

class FFT{
public:
    
    void setup(int _numBands);
    void update();
    void draw();
    
    ofxFFTLive fftLive;
    
    
    int numBands;
    vector<float> fftData, prevFfftData;
    
    //detecting beats
    vector<float> beatVals;
    float beatDecay;
    float minJumpForBeat;
    
    float beatJumpValToCountAsMax;
    float minBeatVal;
    
    bool haveNewHit;
    bool bandsOn[NUM_BANDS];
    
    
};

#endif /* FFT_hpp */
