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

#include "ofxMaxim.h"


#include "BeatMarker.hpp"
#include "StepModeIcons.hpp"

#include "AboutScreen.hpp"
#include "Logo.hpp"

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
#include "SpokesHit.hpp"    //for the arcade machine only

//ios
#include "TouchGeneralButton.hpp"
#include "SoundButton.hpp"

//arcade
#include "ArduinoManager.hpp"



class Sequencer{
public:
    
    void setup();
    //void preHitBeat(void);
    void hitBeat(void);
    void update();
    void draw();
    
    void keyPressed(int key);
    void touchDown(int x, int y);
    void mouseMoved(int x, int y);
    void windowResized(int w, int h);
    void setButtonPositions();
    
    void makeNewHit(int idNum);
    void generateHitObject(int idNum);
    void playSound(int idNum);
    
    void setStepMode(bool isOn);
    void stepModePress(int placeID);
    
    void setRecording(bool isOn);
    
    void clearBeats();
    
    void loadSounds(string filePath);
    
    void loadArcadeSettings(string filePath);
    
    void setVisualEffect();
    
    bool checkIsFirstRun();
    
    bool checkHasRunStepMode();
    void saveHasRunStepMode();
    
    void skipIntro();
    
    void setMaximAudio(bool advanceAudioThisCycle);
    
    void exportBeat();
    void loadBeat(string raw);
    
    //some generla stuff
    bool usingIPad;
    int gameW, gameH;
    
    //states and such?
    bool publicRelease;
    bool autoPlay;
    bool recording;
    //bool turnOnRecordingWhenClearing;
    bool useNumpadKeys;
    bool usePreHitDetection;
    
    //timing
    float deltaTime, prevFrameTime;
    float bpmValue, bpmStartValue;
    
    //drawing
    int whiteVal;
    bool showTouchButtons;
    
    //tracking hits
    vector<Hit *> hits;
    vector<int> hitsToGenerate;
    
    //audio files
#ifdef USING_ARCADE
    #define NUM_SOUNDS 16
#else
    #define NUM_SOUNDS 15
#endif
    
    #define NUM_BEATS 16
    #define NUM_IOS_BEATS_PER_SOUND 8   //this must be a perfect multiple of NUM_BEATS
    
    //maxim stuff
    int		bufferSize;
    int		sampleRate;
    
    double sampleOut;
    
    maxiSample sounds[NUM_IOS_BEATS_PER_SOUND][NUM_SOUNDS];    //on osx, only sounds[0][i] are used. There is no multiplay on iOS, so I am loading far more than needed. This may be a disaster.    PROBABLY DOING THE IOS STYLE ACROSS THE BOARD NOW!
    maxiSample clickTrackSound, clickTrackSound2;
    maxiOsc timer;
    
    bool showWaveForm;
    vector<double> soundData;
    
    //tracking info
    bool useClickTrack;
    
    int thisBeat;
    bool beatsOn[NUM_BEATS][NUM_SOUNDS];
    bool onPreHit;
    float preHitPrc;
    
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
    
    //logo
    Logo logo;
    
    //about screen
    AboutScreen aboutScreen;
    AboutScreen stepModeInstructions;
    
    //doing special stuff the first time the app ever runs
    bool isFirstRun;
    float firstRunTimer, firstRunTime;
    bool hasAddedANote;
    string iosDataPath;
    
    bool hasRunStepMode;
    
    //touch buttons
#define NUM_TOUCH_BUTTONS 15
    TouchGeneralButton touchButtons[NUM_TOUCH_BUTTONS];
    
#define NUM_TOUCH_MENU_BUTTONS 6
#define MENU_BUTTON_LIVE 0
#define MENU_BUTTON_CLICK 1
#define MENU_BUTTON_STEP 2
#define MENU_BUTTON_CLEAR 3
#define MENU_BUTTON_TEMPO_DOWN 4
#define MENU_BUTTON_TEMPO_UP 5
    TouchGeneralButton touchMenuButtons[NUM_TOUCH_MENU_BUTTONS];
    float menuButtonH;
    
    TouchGeneralButton aboutButton;
    ofImage aboutButtonIcon;
    
    //step mode for touch screens
    TouchGeneralButton touchStepButtons[NUM_BEATS];
    SoundButton soundButtons[NUM_SOUNDS];
    
    //taking a screenshot for desktop mode
    bool takeScreenshot;
    
    //arcade console
    bool arcadeMode;
    float arcadeScale;
    float arcadeBeatMarkerDistPrc;
	ofVec2f arcadeOffset;
    ArduinoManager arduino;
    
    float inactivityTimer;
    float inactiveTimeBeforeReset;
    
    //arcade mode tests
    float nextArcadeModeTestWrite;
    
    //uploading beats for making the trailer. not for release
    ofRectangle sendBeatButton;
    string sendBeatStatus;

};

#endif /* Sequencer_hpp */
