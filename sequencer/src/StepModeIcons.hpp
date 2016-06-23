//
//  StepModeIcons.hpp
//  sequencer
//
//  Created by Andrew Wallace on 6/23/16.
//
//

#ifndef StepModeIcons_hpp
#define StepModeIcons_hpp

#include "ofMain.h"

class StepModeIcons{
public:
    
    void draw(int idNum);
    
    void drawTunnel();
    void drawSweep();
    void drawTriangle();
    void drawGrapes();
    void drawBuckshot();
    void drawChaser();
    void drawSlash();
    void drawSquare();
    void drawTrapezoid();
    void drawDotPolygon();
    void drawSizzle();
    void drawDrunkTrianlge();
    void drawCrystal();
    void drawClap();
    void drawWaveColumn();
    
    float topX, topY;
    float w, h;
    
    int whiteVal;
    
};

#endif /* StepModeIcons_hpp */
