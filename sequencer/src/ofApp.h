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
#include "TrapazoidHit.hpp"
#include "DotPolygonHit.hpp"
#include "SizzleHit.hpp"
#include "DrunkTriangleHit.hpp"
#include "CrossBoxHit.hpp"
#include "ClapHit.hpp"
#include "WaveColumnHit.hpp"

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
    
    bool autoPlay;
   
    int whiteVal;
    
    //tracking hits
    vector<Hit *> hits;
    float timeSinceLastHit; //control panel
    float minTimeForNextHit;    //control panel
    
		
};
