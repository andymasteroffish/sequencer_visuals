//
//  SoundButton.hpp
//  sequncer
//
//  Created by Andrew Wallace on 7/1/16.
//
//

#ifndef SoundButton_hpp
#define SoundButton_hpp

#include "ofMain.h"

#include "StepModeIcons.hpp"

class SoundButton{
    
public:
  
    void setup(float x, float y, float w, float h, int _idNum);
    void update(float deltaTime, int curSound);
    void draw();
    bool checkHit(int x, int y);
    void stepModeOn();
    void stepModeOff();
    
    
    int idNum;
    bool isActive;
    ofRectangle hitBox;
    StepModeIcons icons;
    
    float curAlpha;
    
    float onScale;
    float largeScale;
    float curScale;
    
    //expand/shrink when step mode is activated
    bool isGrowing, isShrinking;
    float growTimer;
    float growTime;
    float shrinkTime;
    
    //bounce when tapped
    float bounceTimer;
    float bounceGrowTime;
    float bounceShrinkTime;
    float bounceScale;

    
    
    
    
};

#endif /* SoundButton_hpp */
