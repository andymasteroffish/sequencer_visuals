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
    
    void setup();
    void update(float deltaTime);
    void draw(int idNum, int alpha);
    
    void drawTunnel(int alpha);
    void drawSweep(int alpha);
    void drawTriangle(int alpha);
    void drawGrapes(int alpha);
    void drawBuckshot(int alpha);
    void drawChaser(int alpha);
    void drawSlash(int alpha);
    void drawSquare(int alpha);
    void drawTrapezoid(int alpha);
    void drawDotPolygon(int alpha);
    void drawSizzle(int alpha);
    void drawDrunkTrianlge(int alpha);
    void drawCrystal(int alpha);
    void drawClap(int alpha);
    void drawWaveColumn(int alpha);
    
    float timer;
    
    float topX, topY;
    float w, h;
    
    int whiteVal;
    
};

#endif /* StepModeIcons_hpp */
