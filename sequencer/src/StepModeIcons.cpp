//
//  StepModeIcons.cpp
//  sequencer
//
//  Created by Andrew Wallace on 6/23/16.
//
//

#include "StepModeIcons.hpp"


void StepModeIcons::draw(int idNum){
    
    //defaults
    ofNoFill();
    ofSetColor(0);
    ofSetLineWidth(1);
    
    w = 60;
    h = 60;
    topX = -w/2;
    topY = -h/2;
    
    //testing
    //idNum = 14;
    
    //draw the thing
    if (idNum == 0)     drawTunnel();
    if (idNum == 1)     drawSweep();
    if (idNum == 2)     drawTriangle();
    if (idNum == 3)     drawGrapes();
    if (idNum == 4)     drawBuckshot();
    if (idNum == 5)     drawChaser();
    if (idNum == 6)     drawSlash();
    if (idNum == 7)     drawSquare();
    if (idNum == 8)     drawTrapezoid();
    if (idNum == 9)     drawDotPolygon();
    if (idNum == 10)    drawSizzle();
    if (idNum == 11)    drawDrunkTrianlge();
    if (idNum == 12)    drawCrystal();
    if (idNum == 13)    drawClap();
    if (idNum == 14)    drawWaveColumn();
    
    //guide
//    ofNoFill();
//    ofSetColor(0);
//    ofSetLineWidth(1);
//    ofDrawRectangle(topX, topY, w, h);
}

void StepModeIcons::drawTunnel(){
    float time = fmod( ofGetElapsedTimef() , 0.8);
    ofDrawCircle(0,0, 5 + time*25);
}

void StepModeIcons::drawSweep(){
    float maxTime = 0.5;
    float time = fmod( ofGetElapsedTimef() , maxTime);
    float prc = time/maxTime;
    
    float x = topX + prc * w;
    ofSetLineWidth(2);
    ofDrawLine(x, topY, x, topY+h);
    
    ofSetLineWidth(1);
    int trailLength = 10;
    for (int i=0; i<trailLength; i++){
        int thisX = x + i * -1;
        int numDots = ofMap(i, 0, trailLength, 20, 1);
        
        for (int k=0; k<numDots; k++){
            float thisHeight = ofRandom(1, h*0.5);
            float thisY = ofRandom(topY,topY+h-thisHeight);
            
            ofDrawLine(thisX, thisY, thisX, thisY+thisHeight);
        }
    }
    
    
    
}

void StepModeIcons::drawTriangle(){
    float angle = ofGetElapsedTimef();
    float angleStep = TWO_PI/3;
    float dist = w*0.4;
    ofBeginShape();
    for (int i=0; i<3; i++){
        float xPos = cos(angle+angleStep*i)*dist;
        float yPos = sin(angle+angleStep*i)*dist;
        ofVertex(xPos, yPos);
    }
    ofEndShape(true);
}

void StepModeIcons::drawGrapes(){
    float maxTime = 1;
    float time = fmod( ofGetElapsedTimef() , maxTime);
    
    float maxSize = w*0.25;
    
    float xPos[3] = {w*0.3f, w*0.6f, w*0.8f};
    float yPos[3] = {h*0.2f, h*0.7f, h*0.3f};
    
    for (int i=0; i<3; i++){
        
        float appearTime = i*0.2;
        float growTime = appearTime + 0.1;
        
        float thisSize = 0;
        
        if (time < appearTime){
            thisSize = 0;
        }
        else if (time > appearTime && time <= growTime){
            float prc = (time-appearTime) / (growTime-appearTime);
            prc = powf(prc, 0.5);
            thisSize = prc * maxSize;
        }
        else{
            thisSize = maxSize;
        }
        
        ofDrawCircle(topX+xPos[i], topY+yPos[i], thisSize);
        
    }
    
    
}

void StepModeIcons::drawBuckshot(){
    float maxTime = 1;
    float time = fmod( ofGetElapsedTimef() , maxTime);
    
    int phase = floor(ofGetElapsedTimef());
    
    int numDots = 10;
    float maxSize = w*0.1;
    
    for (int i=0; i<numDots; i++){
        
        float appearTime = maxTime / (float)(numDots *2);
        float growTime = appearTime + 0.1;
        
        float thisSize = 0;
        
        if (time < appearTime){
            thisSize = 0;
        }
        else if (time > appearTime && time <= growTime){
            float prc = (time-appearTime) / (growTime-appearTime);
            prc = powf(prc, 0.5);
            thisSize = prc * maxSize;
        }
        else{
            thisSize = maxSize;
        }
        
        float padding = w*0.1;
        float xPos = topX+padding + ofNoise(phase, i*12.3, 0) * (w-padding*2);
        float yPos = topY+padding + ofNoise(phase, i*31.9, 71) * (h-padding*2);
        
        ofDrawCircle(xPos, yPos, thisSize);
        
    }
    
}

void StepModeIcons::drawChaser(){
    float maxTime = 0.7;
    float time = fmod( ofGetElapsedTimef() , maxTime);
    float prc = time/maxTime;
    
    int numPoints = 15;
    float angleStep = TWO_PI/numPoints;
    float dist = w*0.45;
    
    int curHighligh = numPoints*prc;
    
    for (int i=0; i<numPoints; i++){
        
        if (curHighligh==i){
            ofSetLineWidth(4);
        }else{
            ofSetLineWidth(1);
        }
        
        float angle1 = i*angleStep;
        float angle2 = (i+1)*angleStep;
        
        float x1 = cos(angle1) * dist;
        float y1 = sin(angle1) * dist;
        
        float x2 = cos(angle2) * dist;
        float y2 = sin(angle2) * dist;
        
        ofDrawLine(x1,y1, x2,y2);
    }
    
}

void StepModeIcons::drawSlash(){
    float maxTime = 0.7;
    float time = fmod( ofGetElapsedTimef() , maxTime);
    
    float angles[] = {PI/4, TWO_PI*0.8};
    ofVec2f center(0,0);
    
    ofSetLineWidth(4);
    
    //draw the center
    ofFill();
    ofDrawCircle(0, 0, w*0.13);
    
    float lineDist = w*0.5;
    for (int i=0; i<2; i++){
        
        float startTime = i*0.2;
        float endTime = startTime + 0.2;
        
        float prc = 0;
        if (time > startTime && prc <= endTime){
            prc = (time-startTime)/(endTime-startTime);
        }
        if (time > endTime){
            prc = 1;
        }
        
        float pntX = center.x + sin(angles[i]) * lineDist;
        float pntY = center.y + cos(angles[i]) * lineDist;
        
        ofVec2f startPnt(pntX, pntY);
        ofVec2f endPnt = center;
        if (i==1){
            startPnt = center;
            endPnt.set(pntX, pntY);
        }
        
        ofVec2f actualEnd = (1-prc) * startPnt + prc * endPnt;
        
        ofDrawLine(startPnt.x, startPnt.y, actualEnd.x, actualEnd.y);
        
    }
    
    
}

void StepModeIcons::drawSquare(){
    float maxTime = 0.9;
    float time = fmod( ofGetElapsedTimef() , maxTime);
    float prc = CLAMP(time/(maxTime*0.5), 0, 1);
    prc = powf(prc, 2);
    
    float startAngle = 0;
    float endAngle = 130;
    float thisAngle = (1-prc)*startAngle + prc * endAngle;
    
    float rectSize = w*0.4;
    
    ofPushMatrix();
    ofTranslate(-w*0.12, 0);
    ofRotate(thisAngle);
    
    ofSetLineWidth(2);
    ofDrawRectangle(0, -rectSize, rectSize, rectSize);
    
    ofPopMatrix();
    
    
    
}

void StepModeIcons::drawTrapezoid(){
    
    float maxTime = 0.6;
    float time = fmod( ofGetElapsedTimef() , maxTime);
    float prc = time/maxTime;
    prc = powf(prc, 1.3);
    
    float farY = topY + prc * h;
    float smallW = w*0.3;
    float largeW = w*0.5;
    
    ofBeginShape();
    
    ofVertex(-smallW, 0);
    ofVertex(smallW, 0);
    
    ofVertex(largeW, farY);
    ofVertex(-largeW, farY);
    
    ofEndShape(true);
    
    
    
}

void StepModeIcons::drawDotPolygon(){
    
    int numDots = 5;
    ofVec2f pnts[5];
    
    for (int i=0; i<numDots; i++){
        
        float padding = w*0.1;
        float speed = 0.3;
        pnts[i].x = topX+padding + ofNoise(i, 10, ofGetElapsedTimef()*speed) * (w-padding*2);
        pnts[i].y = topY+padding + ofNoise(i, 100, ofGetElapsedTimef()*speed) * (h-padding*2);
    }
    
    ofSetColor(0);
    ofSetLineWidth(1);
    for (int i=1; i<numDots; i++){
        ofDrawLine(pnts[i-1].x, pnts[i-1].y, pnts[i].x, pnts[i].y);
    }
    ofDrawLine(pnts[0].x, pnts[0].y, pnts[numDots-1].x, pnts[numDots-1].y);
    
    
    float circleSize = w*0.1;
    for (int i=0; i<numDots; i++){
        ofSetColor(whiteVal);
        ofFill();
        ofDrawCircle(pnts[i].x, pnts[i].y, circleSize);
        
        ofSetColor(0);
        ofSetLineWidth(1);
        ofNoFill();
        ofDrawCircle(pnts[i].x, pnts[i].y, circleSize);
    }
    
}

void StepModeIcons::drawSizzle(){
    
    int numDots = 15;
    ofVec2f pnts[15];
    
    float ySpacing = h/numDots;
    float xSpacing = (w/numDots) * 0.5;
    
    float noiseSpeed = 0.5;
    float noiseRange = w*0.05;
    
    for (int i=0; i<numDots; i++){
        float xDist = (xSpacing * i) + (1-ofNoise(ofGetElapsedTimef()*noiseSpeed, i, 0)*2)*noiseRange;
        pnts[i].x = topX+w*0.5 + xDist * (i%2==0 ? 1 : -1);
        
        pnts[i].y = topY+h-(ySpacing * i) + (1-ofNoise(ofGetElapsedTimef()*noiseSpeed, i, 100)*2)*noiseRange;
        
        if (i!=0){
            ofDrawLine(pnts[i], pnts[i-1]);
        }
    }
    
}

void StepModeIcons::drawDrunkTrianlge(){
    
    float maxTime = 1.2;
    float time = fmod( ofGetElapsedTimef() , maxTime);
    
    int numTriangles = 4;
    float curAngle = PI*0.1;
    float angleStep = -PI * 0.2;
    float distStep = w*0.3;
    
    ofVec2f pos( topX+w*0.22, topY+h*0.85);
    
    float maxSize = w*0.15;
    
    for (int i=0; i<numTriangles; i++){
        
        float appearTime = i * 0.2;
        float growTime = appearTime + 0.2;
        
        float thisSize = 0;
        
        if (time < appearTime){
            thisSize = 0;
        }
        else if (time > appearTime && time <= growTime){
            float prc = (time-appearTime) / (growTime-appearTime);
            prc = powf(prc, 0.5);
            thisSize = prc * maxSize;
        }
        else{
            thisSize = maxSize;
        }

        
        ofSetCircleResolution(3);
        ofPushMatrix();
        ofTranslate(pos.x, pos.y);
        ofRotate( ofRadToDeg(curAngle));
        
        ofDrawCircle(0,0, thisSize);
        
        ofPopMatrix();
        
        curAngle += angleStep;
        pos.x += cos(curAngle) * distStep;
        pos.y += sin(curAngle) * distStep;
        
    }
    
}

void StepModeIcons::drawCrystal(){
    
    float angle = ofGetElapsedTimef();
    float angleStep = TWO_PI/3;
    float dist[3] = {w*0.4f, w*0.3f, w*0.48f};
    ofBeginShape();
    for (int i=0; i<3; i++){
        float xPos = cos(angle+angleStep*i)*dist[i];
        float yPos = sin(angle+angleStep*i)*dist[i];
        ofVertex(xPos, yPos);
    }
    ofEndShape(true);
    
    for (int i=0; i<3; i++){
        float xPos = cos(angle+angleStep*i)*dist[i];
        float yPos = sin(angle+angleStep*i)*dist[i];
        ofDrawLine(0, 0, xPos, yPos);
    }
}

void StepModeIcons::drawClap(){
    float maxTime = 0.6;
    float time = fmod( ofGetElapsedTimef() , maxTime);
    float prc = time/maxTime;
    prc = powf(prc, 2.2);
    
    float angle = (1-prc) * PI/4;
    
    float pntDist = w*0.65;
    
    ofVec2f bottom( topX+w*0.5, topY+ pntDist + h*0.1);
    
    
    ofVec2f pntA, pntB;
    
    pntA.x = bottom.x + cos(angle - PI/2) * pntDist;
    pntA.y = bottom.y + sin(angle - PI/2) * pntDist;
    
    pntB.x = bottom.x + cos(-angle - PI/2) * pntDist;
    pntB.y = bottom.y + sin(-angle - PI/2) * pntDist;
    

    ofSetLineWidth(2);
    ofDrawLine(bottom, pntA);
    ofDrawLine(bottom, pntB);
    
}

void StepModeIcons::drawWaveColumn(){
    
    int numPnts = 50;
    ofVec2f pnts[50];
    ofVec2f basePnts[50];
    
    float boxW = w*0.5;
    float boxH = h*0.9;
    float pntSpacing = boxH/(numPnts/2);
    
    float waveZoom = 0.6;
    float waveSpeed = 1;
    float waveAmount = w*0.1;
    
    //set the base points
    for (int i=0; i<numPnts/2; i++){
        basePnts[i].x = -boxW/2;
        basePnts[i].y = -boxH/2 + pntSpacing * i;
        
        basePnts[numPnts/2+i].x = boxW/2;
        basePnts[numPnts/2+i].y = boxH/2 - pntSpacing * (i+1);
    }
    
    //go through each point starting at the tops
    for (int i=0; i<numPnts/2; i++){
        float shift = sin( (float)i*waveZoom + ofGetElapsedTimef()*waveSpeed) * waveAmount;
        pnts[i].x             = basePnts[i].x + shift;
        pnts[numPnts/2+i].x   = basePnts[numPnts/2+i].x + shift;
        
        pnts[i].y = basePnts[i].y;
        pnts[numPnts/2+i].y   = basePnts[numPnts/2+i].y;
    }
    
    
    //draw
    for (int i=0; i<numPnts-1; i++){
        ofDrawLine(pnts[i], pnts[i+1]);
    }
    ofDrawLine(pnts[0], pnts[ numPnts-1]);
}