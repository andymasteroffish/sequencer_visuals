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
    void draw(int idNum, float lineWidthPrc, int alpha);
    
    void drawTunnel(float lineWidthPrc, int alpha);
    void drawSweep(float lineWidthPrc, int alpha);
    void drawTriangle(float lineWidthPrc, int alpha);
    void drawGrapes(float lineWidthPrc, int alpha);
    void drawBuckshot(float lineWidthPrc, int alpha);
    void drawSpokes(float lineWidthPrc, int alpha);
    void drawChaser(float lineWidthPrc, int alpha);
    void drawSlash(float lineWidthPrc, int alpha);
    void drawSquare(float lineWidthPrc, int alpha);
    void drawTrapezoid(float lineWidthPrc, int alpha);
    void drawDotPolygon(float lineWidthPrc, int alpha);
    void drawSizzle(float lineWidthPrc, int alpha);
    void drawDrunkTrianlge(float lineWidthPrc, int alpha);
    void drawCrystal(float lineWidthPrc, int alpha);
    void drawClap(float lineWidthPrc, int alpha);
    void drawWaveColumn(float lineWidthPrc, int alpha);
    
    float timer;
    
    float topX, topY;
    float w, h;
    
    int whiteVal;
    
};

#endif /* StepModeIcons_hpp */
