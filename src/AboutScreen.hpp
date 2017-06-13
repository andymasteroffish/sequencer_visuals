//
//  AboutScreen.hpp
//  sequencer
//
//  Created by Andrew Wallace on 7/9/16.
//
//

#ifndef AboutScreen_hpp
#define AboutScreen_hpp

#include "ofMain.h"
#include "BounceWord.hpp"
#include "FillCircle.hpp"
#include "SystemSpecificInfo.h"

class AboutScreen{
public:
    
    void setup(int _whiteVal, bool usingiPad, bool isStepModeInfo, string _versionText);
    void setupAbout();
    void setupStepMode();
    
    void turnOn();
    void turnOff();
    void update(float deltaTime);
    void draw();
    
    void createWordsFromLine(string line, float x, float y, ofTrueTypeFont * font);
    
    bool canTurnOff();
    
    bool isActive;
    bool isAnimating;
    
    ofRectangle box;
    float ipadAdjust;
    
    int whiteVal;
    
    float bgFade;
    
    float timer;
    float minTimeOn;
    
    
    ofTrueTypeFont titleFont, textFont;
    vector<BounceWord> words;
    string verb;
    
    vector<FillCircle> bgCircles;
    
    string versionText;
};

#endif /* AboutScreen_hpp */
