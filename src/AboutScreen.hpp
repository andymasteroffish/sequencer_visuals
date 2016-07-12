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
    
    void setup(int _whiteVal, bool usingiPad);
    void turnOn();
    void turnOff();
    void update(float deltaTime);
    void draw();
    
    void createWordsFromLine(string line, float x, float y, ofTrueTypeFont * font);
    
    bool isActive;
    bool isAnimating;
    
    ofRectangle box;
    
    int whiteVal;
    
    float bgFade;
    
    
    
    ofTrueTypeFont titleFont, textFont;
    vector<BounceWord> words;
    
    vector<FillCircle> bgCircles;
    
};

#endif /* AboutScreen_hpp */
