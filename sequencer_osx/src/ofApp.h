#pragma once

#include "ofMain.h"

#include "Sequencer.hpp"

class ofApp : public ofBaseApp{

public:
    
    void setup();
    void update();
    void draw();
    
    void mousePressed(int x, int y, int button);
    void mouseMoved(int x, int y );

    void keyPressed(int key);
    void windowResized(int w, int h);
    
    void audioOut(float * output, int bufferSize, int nChannels);
    
    Sequencer sequencer;
    
    
    //testing
    //ofArduino ard;
private:
    //void setupArduino(const int & version);
		
};
