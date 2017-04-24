//
//  Logo.hpp
//  sequencer
//
//  Created by Andrew Wallace on 3/11/17.
//
//

#ifndef Logo_hpp
#define Logo_hpp

#include "ofMain.h"

class Logo{
public:
    
    void setup(int _whiteVal, bool usingiPad, ofTrueTypeFont * _font);
    void update(float deltaTime);
    
    void draw();
    
    void drawLogoImages();
    void drawHeadphoneMessage();
    
    float masterScale;
    int whiteVal;
    
    float timer;
    
    float growTime, pauseTime, fadeTime;
    
    float prcComplete;
    
    ofVec2f logoPos;
    
    ofTrueTypeFont * font;
    
    //testing
    ofImage logoPic;
    
    #define LOGO_SIZE 9
    ofImage logoPics[LOGO_SIZE];
    float picW, picH;
};

#endif /* Logo_hpp */
