//
//  Sequencer.hpp
//  sequencer
//
//  Created by Andrew Wallace on 6/28/16.
//
//

#ifndef Sequencer_hpp
#define Sequencer_hpp


#include "SystemSpecificInfo.h"

#include "ofMain.h"

#include "Bpm.hpp"

#include "BeatMarker.hpp"
#include "StepModeIcons.hpp"

#include "AboutScreen.hpp"

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

//ios
#include "TouchGeneralButton.hpp"
#include "SoundButton.hpp"



class Sequencer{
public:
    
    void setup();
    void preHitBeat(void);
    void hitBeat(void);
    void update();
    void draw();
    
    void keyPressed(int key);
    void touchDown(int x, int y);
    void windowResized(int w, int h);
    void setButtonPositions();
    
    void makeNewHit(int idNum);
    
    void setStepMode(bool isOn);
    void stepModePress(int placeID);
    
    void setRecording(bool isOn);
    
    void clearBeats();
    
    void loadSounds(string filePath);
    
    void setVisualEffect();
    
    bool checkIsFirstRun();
    
    //some generla stuff
    bool usingIPad;
    int gameW, gameH;
    
    //states and such?
    bool publicRelease;
    bool autoPlay;
    bool recording;
    bool turnOnRecordingWhenClearing;
    bool useNumpadKeys;
    bool usePreHitDetection;
    
    //timing
    float deltaTime, prevFrameTime;
    
    Bpm bpm;
    float bpmValue, bpmStartValue;
    
    //drawing
    int whiteVal;
    bool showHelp;
    bool showTouchButtons;
    
    //tracking hits
    vector<Hit *> hits;
    
    //audio files
    #define NUM_BEATS 16
    #define NUM_SOUNDS 15
    #define NUM_IOS_BEATS_PER_SOUND 8   //this must be a perfect multiple of NUM_BEATS
    
    ofSoundPlayer sounds[NUM_IOS_BEATS_PER_SOUND][NUM_SOUNDS];    //on osx, only sounds[0][i] are used. There is no multiplay on iOS, so I am loading far more than needed. This may be a disaster
    ofSoundPlayer clickTrackSound;
    bool useClickTrack;
    
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
    
    //fucking around with visuals
    int visualEffectNum;
    ofShader shader;
    
    bool doDisplacement;
    bool doColorFade;
    ofColor colorA, colorB;
    
    bool doCamMovement;
    
    //fonts
    ofTrueTypeFont buttonFont, buttonFontSmall;
    
    //about screen
    AboutScreen aboutScreen;
    
    //doing special stuff the first time the app ever runs
    bool isFirstRun;
    float firstRunTimer;
    string iosDataPath;
    
    //touch buttons
#define NUM_TOUCH_BUTTONS 15
    TouchGeneralButton touchButtons[NUM_TOUCH_BUTTONS];
#define NUM_TOUCH_MENU_BUTTONS 5
    TouchGeneralButton touchMenuButtons[NUM_TOUCH_MENU_BUTTONS];
    float menuButtonH;
    
    TouchGeneralButton aboutButton;
    ofImage aboutButtonIcon;
    
    //step mode for touch screens
    TouchGeneralButton touchStepButtons[NUM_BEATS];
    SoundButton soundButtons[NUM_SOUNDS];
};

#endif /* Sequencer_hpp */
