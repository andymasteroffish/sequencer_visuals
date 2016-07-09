//
//  AboutScreen.cpp
//  sequencer
//
//  Created by Andrew Wallace on 7/9/16.
//
//

#include "AboutScreen.hpp"


void AboutScreen::setup(int _whiteVal, bool usingiPad){
    
    whiteVal = _whiteVal;
    
    box.width = 800;
    box.height = 600;
    box.x = ofGetWidth()/2 - box.width/2;
    box.y = ofGetHeight()/2 - box.height/2;
    
    textFont.load("Futura.ttf", 20);
    
    isActive = false;
    isAnimating = false;
    
    float textX = box.x + 25;
    float textY = 300;
    float textYSpacing = 40;
    
    bgFade = 0;
    
    
    createWordsFromLine("Tap anywhere to add a sound to the beat.", textX, textY+=textYSpacing, &textFont);
    createWordsFromLine("Step mode allows you to place specific sounds.", textX, textY+=textYSpacing, &textFont);
    createWordsFromLine("Tapping Live will allow you to play sounds without recording them.", textX, textY+=textYSpacing, &textFont);
    
    bgCircles.resize(10);
}

void AboutScreen::turnOn(){
    isActive = true;
    
    for (int i=0; i<words.size(); i++){
        words[i].reset();
    }
    
    float padding = 120;
    for (int i=0; i<bgCircles.size(); i++){
        bgCircles[i].setup(ofRandom(box.x+padding, box.x+box.width-padding), ofRandom(box.y+padding, box.y+box.height-padding), box);
    }
}
void AboutScreen::turnOff(){
    isActive = false;
    isAnimating = true;
    
    for (int i=0; i<words.size(); i++){
        words[i].close();
    }
    for (int i=0; i<bgCircles.size(); i++){
        bgCircles[i].close();
    }
}

void AboutScreen::update(float deltaTime){
    //DON'T UPDATE EVERYTHING IF IT IS NOT BEING SEEN
    if (!isActive && !isAnimating){
        return;
    }
    
    for (int i=0; i<bgCircles.size(); i++){
        bgCircles[i].update(deltaTime);
    }
    
    for (int i=0; i<words.size(); i++){
        words[i].update(deltaTime);
    }
    
    float targetBgFade = isActive ? 170 : 0;
    float bgXeno = 0.9;
    bgFade = bgXeno * bgFade + (1-bgXeno) * targetBgFade;
}

void AboutScreen::draw(){
    
    //DON'T DRAW EVERYTHING IF IT IS NOT BEING SEEN
    if (!isActive && !isAnimating){
        return;
    }
    
    ofEnableAlphaBlending();
    //bg fade
    ofSetColor(whiteVal, bgFade);
    ofFill();
    ofDrawRectangle(0,0, ofGetWidth(), ofGetHeight());
    
    //box
    ofSetColor(255, 210);
//    ofDrawRectangle(box);
    for (int i=0; i<bgCircles.size(); i++){
        bgCircles[i].draw();
    }
    ofDisableAlphaBlending();
    
    //testing
//    ofNoFill();
//    ofSetColor(0);
//    ofDrawRectangle(box);
    
    ofSetColor(0);
    for (int i=0; i<words.size(); i++){
        words[i].draw();
    }
}

void AboutScreen::createWordsFromLine(string line, float x, float y, ofTrueTypeFont * font){
    
    float spaceW = font->stringWidth("_") * 0.8;
    //cout<<"space "<<spaceW<<endl;
    
    //break the line up on space
    vector<string> individualWords;
    string curWord = "";
    for (int i=0; i<line.size(); i++){
        if (line[i] == ' '){
            individualWords.push_back(curWord);
            curWord = "";
        }
        else{
            curWord += line[i];
        }
    }
    if (curWord.size() > 0){
        individualWords.push_back(curWord);
    }
    
    //start placing them
    float curX = x;
    for (int i=0; i<individualWords.size(); i++){
        BounceWord thisWord;
        thisWord.setup(individualWords[i], curX, y, font);
        curX += font->stringWidth(individualWords[i]) + spaceW;
        words.push_back(thisWord);
    }
}