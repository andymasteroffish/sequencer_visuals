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
    
    Sequencer sequencer;
    
    
    //    void keyReleased(int key);
    //    void mouseMoved(int x, int y );
    //    void mouseDragged(int x, int y, int button);
    //    void mousePressed(int x, int y, int button);
    //    void mouseReleased(int x, int y, int button);
    //    void mouseEntered(int x, int y);
    //    void mouseExited(int x, int y);
    //    void dragEvent(ofDragInfo dragInfo);
    //    void gotMessage(ofMessage msg);
    
		
};
