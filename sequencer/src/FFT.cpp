//
//  FFT.cpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#include "FFT.hpp"


void FFT::setup(int _numBands){
    numBands = _numBands;
    
    //settup fft
    fftLive.setMirrorData(false);
    fftLive.setup();
    
    int bufferSize = numBands*4;
    fftLive.setBufferSize(bufferSize);
    
    for (int i=0; i<numBands; i++){
        beatVals.push_back(1);
    }
    beatDecay = 0.9;
    minJumpForBeat = 0.2;
    beatJumpValToCountAsMax = 0.7;
    minBeatVal = 0.65;
    
}

void FFT::update(){
    
    fftLive.update();
    
    
    
    prevFfftData.clear();
    for (int i=0; i<fftData.size(); i++){
        prevFfftData.push_back( fftData[i] );
    }
    fftData = fftLive.getFftPeakData();
    
    //check for "beats"
    haveNewHit = false;
    for (int i=0; i<NUM_BANDS; i++){
        bandsOn[i] = false;
    }
    
    if (prevFfftData.size() >= beatVals.size()){
        for (int i=0; i<beatVals.size(); i++){
            beatVals[i] *= beatDecay;
            if (fftData[i] > prevFfftData[i]) {
                float fftDif = fftData[i] - prevFfftData[i];
                if (fftDif >= minJumpForBeat){
                    beatVals[i] = 1;//ofMap(fftDif, minJumpForBeat, beatJumpValToCountAsMax, minBeatVal, 1, true);
                    bandsOn[i] = true;
                    haveNewHit = true;
                }
                
            }
        }
    }
    
    
}

void FFT::draw(){
    
    //drawing info
    int totalWidth = 512;
    int totalHeight = 256;
    int barWidth = 512 / numBands;
    
    ofPushMatrix();
    
    ofTranslate(10, ofGetHeight() - 10);
    
    ofNoFill();
    ofSetColor(0);
    ofDrawRectangle(0, 0, totalWidth, -totalHeight);
    
    
    for(int i=0; i<numBands; i++) {
        ofFill();
        ofSetColor(100,100,210);
        ofDrawRectangle(i * barWidth, 0, barWidth/2, -fftData[i] * totalHeight);
        ofNoFill();
        ofSetColor(0);
        ofDrawRectangle(i * barWidth, 0, barWidth/2, -fftData[i] * totalHeight);
        
        ofFill();
        ofSetColor(210,100,100);
        ofDrawRectangle(i * barWidth + barWidth/4, 0, barWidth/2, -beatVals[i] * totalHeight);
        ofNoFill();
        ofSetColor(0);
        ofDrawRectangle(i * barWidth + barWidth/4, 0, barWidth/2, -beatVals[i] * totalHeight);
        
        if (beatVals[i] > 0.85){
            ofSetColor(210,100,100);
            ofFill();
            ofDrawCircle(i*barWidth+barWidth/2, -totalHeight-barWidth, barWidth/2);
        }
        
    }
    
    ofPopMatrix();
    
    
    fftLive.draw();
}