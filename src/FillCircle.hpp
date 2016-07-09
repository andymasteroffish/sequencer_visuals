//
//  FillCircle.hpp
//  sequencer
//
//  Created by Andrew Wallace on 7/9/16.
//
//

#ifndef FillCircle_hpp
#define FillCircle_hpp

#include "ofMain.h"

class FillCircle{
public:
    
    void setup(float x, float y, ofRectangle _bounds);
    void close();
    void update(float deltaTime);
    void draw();
    
    ofVec2f center;
    
    float curSize;
    float maxSize;
    float growthSpeed;
    
    int numPoints;
    float angleStep;
    
    ofRectangle bounds;
    
    float cornerSize;
    ofVec2f topLeft, topRight, botLeft, botRight;
    
    bool isClosing;
    float closeTimer;
    
};

#endif /* FillCircle_hpp */
