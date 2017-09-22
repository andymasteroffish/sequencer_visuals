//
//  SpokesHit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 9/22/17.
//
//

#include "SpokesHit.hpp"


void SpokesHit::setupCustom(){
    
    shrinkTime = 1;
    rotTime = shrinkTime - 0.7;
    killTime = shrinkTime + 0.4;
    
    float armLength = 50;
    
    float padding = 20 + armLength;
    pos.set( ofRandom(padding, gameW-padding), ofRandom(padding,gameH-padding));
    
    
    if (arcadeMode){
        pos = getArcadePoint(arcadeModeDist);
    }
    
    int numSpokes = ofRandom(5,8);
    float angleSection = TWO_PI/(float)numSpokes;
    for (int i=0; i<numSpokes; i++){
        Spoke spoke;
        spoke.setup(armLength, angleSection * i);
        spokes.push_back(spoke);
    }
}
void SpokesHit::updateCustom(){
    
    //suck 'em back in after a bit
    if (timer > shrinkTime){
        for (int i=0; i<spokes.size(); i++){
            spokes[i].setTargetDist(0);
        }
    }
    
    //rotate 'em
    float rotSpeed = PI*0.75;
    float easeInTime = 0.3;
    for (int i=0; i<spokes.size(); i++){
        float rotStartTime = rotTime + 0.05 * i;
        if (timer > rotStartTime){
            float prc = MIN( (timer-rotStartTime)/easeInTime, 1);
            spokes[i].angle += rotSpeed * prc * deltaTime;
        }
    }
    
    //update the spokes
    for (int i=0; i<spokes.size(); i++){
        spokes[i].update(timer);
    }
    
    //time to die?
    if (timer > killTime){
        killMe = true;
    }
    
}
void SpokesHit::draw(){
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
   
    ofSetLineWidth(1);
    for (int i=0; i<spokes.size(); i++){
        spokes[i].drawLine();
    }
    
    setLineWidth(2);
    ofSetCircleResolution(5);
    ofNoFill();
    for (int i=0; i<spokes.size(); i++){
        spokes[i].drawCircle();
    }
    ofPopMatrix();
}
