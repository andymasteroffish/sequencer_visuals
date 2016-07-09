//
//  BounceWord.hpp
//  sequencer
//
//  Created by Andrew Wallace on 7/9/16.
//
//

#ifndef BounceWord_hpp
#define BounceWord_hpp

#include "ofMain.h"

class BounceWord{
public:
    
    void setup(string _text, float x, float y, ofTrueTypeFont * _font);
    void reset();
    void close();
    void update(float deltaTime);
    void draw();
    
    float timer;
    float growTime;
    float shrinkXeno;
    float largeScale;
    
    float curScale;
    
    ofTrueTypeFont * font;
    
    ofVec2f pos;
    string text;
    
    float textW, textH;
    
    bool isGrowing;
    
};

#endif /* BounceWord_hpp */
