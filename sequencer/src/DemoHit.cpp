//
//  DemoHit.cpp
//  sequencer
//
//  Created by Andrew Wallace on 5/24/16.
//
//

#include "DemoHit.hpp"


void DemoHit::setupCustom(){
    pos.x = ofGetWidth()-200;
    pos.y = ofGetHeight()-100;
    
    riseSpeed = 10;
    
    
}

void DemoHit::updateCustom(){
    
    pos.y -= riseSpeed;
    
    if (pos.y < -50){
        killMe = true;
    }
}

void DemoHit::draw(){
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    
//    for (int i=0; i<NUM_BANDS/2; i++){
//        float circleSize = 10;
//        ofSetColor(210,100,100);
//        if (bandsOnMicro[i]){
//            ofFill();
//        }else{
//            ofNoFill();
//        }
//        ofSetCircleResolution(30);
//        ofDrawCircle((circleSize*2)*i, 0, circleSize);
//    }
    
    ofPopMatrix();
}