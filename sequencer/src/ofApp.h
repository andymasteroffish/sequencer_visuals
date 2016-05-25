#pragma once

#include "ofMain.h"

#include "FFT.hpp"

#include "Hit.hpp"
#include "DemoHit.hpp"
#include "TunnelHit.hpp"
#include "SweepHit.hpp"
#include "TriangleHit.hpp"
#include "GrapesHit.hpp"
#include "BuckshotHit.hpp"
#include "ChaserHit.hpp"
#include "SlashHit.hpp"
#include "SquareHit.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void makeNewHit(bool bandsOn[NUM_BANDS]);
    void makeNewTestHit(int idNum);
    
    float deltaTime, prevFrameTime;
    
    FFT fft;
    bool showFFT;
    
   
    
    //tracking hits
    vector<Hit *> hits;
    float timeSinceLastHit; //control panel
    float minTimeForNextHit;    //control panel
		
};
