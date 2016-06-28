#pragma once

#include "ofMain.h"

#include "ofxBpm.h"

#include "BeatMarker.hpp"
#include "StepModeIcons.hpp"

#include "Hit.hpp"
#include "TunnelHit.hpp"
#include "SweepHit.hpp"
#include "TriangleHit.hpp"
#include "GrapesHit.hpp"
#include "BuckshotHit.hpp"
#include "ChaserHit.hpp"
#include "SlashHit.hpp"
#include "SquareHit.hpp"
#include "TrapezoidHit.hpp"
#include "DotPolygonHit.hpp"
#include "SizzleHit.hpp"
#include "DrunkTriangleHit.hpp"
#include "CrystalHit.hpp"
#include "ClapHit.hpp"
#include "WaveColumnHit.hpp"

class ofApp : public ofBaseApp{

public:
    void setup();
    void preHitBeat(void);
    void hitBeat(void);
    void update();
    void draw();

    void keyPressed(int key);
    void windowResized(int w, int h);
    
    void makeNewHit(int idNum);
    void stepModePress(int placeID);
    
    void clearBeats();
    
    void loadSounds(string filePath);
    
    void setVisualEffect();
    
    //states and such?
    bool publicRelease;
    bool autoPlay;
    bool recording;
    bool turnOnRecordingWhenClearing;
    bool useNumpadKeys;
    bool usePreHitDetection;
    
    //timing
    float deltaTime, prevFrameTime;
    
    //ofxBPM tools by mirrorboy714
    //https://github.com/mirrorboy714/ofxBpm
    int bpmValue, bpmStartValue;
    ofxBpm bpm;
    
    //drawing
    int whiteVal;
    bool showHelp;
    
    //tracking hits
    vector<Hit *> hits;
    
    //audio files
#define NUM_SOUNDS 15
    ofSoundPlayer sounds[NUM_SOUNDS];
    
#define NUM_BEATS 16
    int thisBeat;
    bool beatsOn[NUM_BEATS][NUM_SOUNDS];
    bool onPreHit;
    
    //float beatXPadding,
    float beatXSpacing;
    float beatYDistFromBottom;
    BeatMarker beatMarkers[NUM_BEATS];
    
    //step mode?
    bool stepMode;
    int curStepSound;
    StepModeIcons stepModeIcons;
    
    //fucking around with visuals
    int visualEffectNum;
    ofShader shader;
    
    bool doDisplacement;
    bool doColorFade;
    ofColor colorA, colorB;
    
    bool doCamMovement;
    
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
