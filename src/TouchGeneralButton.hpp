//
//  TouchGeneralButton.hpp
//  sequncer
//
//  Created by Andrew Wallace on 6/28/16.
//
//

#ifndef TouchGeneralButton_hpp
#define TouchGeneralButton_hpp

#include "ofMain.h"

class TouchGeneralButton{
public:
    
    
    void setup(int _x, int _y, int _width, int _height);
    void setText(string _text, ofTrueTypeFont * _font);
    void update(float deltaTime);
    void draw();
    bool checkHit(int x, int y);
    
    ofRectangle box;
//    ofVec2f pos;
//    float width;
//    float height;
    
    float curFadeVal;
    float fadeSpeed;
    
    ofColor fullColor;
    
    //some buttons can have text
    string text;
    ofTrueTypeFont * font;
    
};

#endif /* TouchGeneralButton_hpp */
