//
//  AboutScreen.cpp
//  sequencer
//
//  Created by Andrew Wallace on 7/9/16.
//
//

#include "AboutScreen.hpp"


void AboutScreen::setup(int _whiteVal, bool usingiPad, bool isStepModeInfo, string _versionText){
    
    words.clear();
    
    ipadAdjust = usingiPad ? 1.5 : 1;
    
    whiteVal = _whiteVal;
    
    versionText = _versionText;
    
    if (textFont.isLoaded() == false){
        if (!isStepModeInfo){
            titleFont.load("Futura.ttf", 40 * ipadAdjust);
            textFont.load("Futura.ttf", 20 * ipadAdjust);
        }else{
            textFont.load("Futura.ttf", 20 * ipadAdjust);
        }
    }
    
    verb = "Clicking";
#ifdef USING_IOS
    verb = "Tapping";
#endif
    
    isActive = false;
    isAnimating = false;
    
    bgFade = 0;
    
    minTimeOn = 2.5;
    
    if (!isStepModeInfo){
        setupAbout();
    }else{
        setupStepMode();
    }
    
    string dismissMessage = verb+" anywhere will dismiss this";
    float dismissX = box.x+box.width/2 - textFont.stringWidth(dismissMessage)/2;
    createWordsFromLine(dismissMessage, dismissX, box.y+box.height-20*ipadAdjust, &textFont);
    
    if (bgCircles.size() == 0){
        bgCircles.resize(10);
    }
}

void AboutScreen::setupAbout(){
    box.width = 800 * ipadAdjust;
    box.height = 600 * ipadAdjust;
    box.x = ofGetWidth()/2 - box.width/2;
    box.y = ofGetHeight()/2 - box.height/2;
    
    float titleY = box.y+60*ipadAdjust;
    float titleX = box.x+box.width/2 - titleFont.stringWidth("Bleep Space")/2;
    createWordsFromLine("Bleep Space", titleX,titleY, &titleFont);
    
    createWordsFromLine(versionText, box.x+box.width -textFont.stringWidth(versionText)*2, titleY, &textFont);
    
    float byLine1X = box.x+box.width/2 - textFont.stringWidth("by Andy Wallace")/2;
    createWordsFromLine("by Andy Wallace", byLine1X, titleY + 40*ipadAdjust, &textFont);
    
    float byLine2X = box.x+box.width/2 - textFont.stringWidth("Sounds by Dan Friel")/2;
    createWordsFromLine("Sounds by Dan Friel", byLine2X, titleY + 75*ipadAdjust, &textFont);
    
    float textX = box.x + 25*ipadAdjust;
    float textY = box.y + 155*ipadAdjust;
    float textYSpacing = 40*ipadAdjust;
    float textYBreak = 25*ipadAdjust;
    
    
    
    createWordsFromLine(verb+" anywhere will add a sound to the beat.", textX, textY+=textYSpacing, &textFont);
    createWordsFromLine("Step mode allows you to place specific sounds.", textX, textY+=textYSpacing, &textFont);
    //createWordsFromLine(verb+" Live will allow you to play sounds without recording them.", textX, textY+=textYSpacing, &textFont);
    
    textY+=textYBreak;
    
    string otherVersion = "See PDF for keyboard controls. iOS version available.";
#ifdef USING_IOS
    otherVersion = "PC/Mac version available at itch.io/andymakes.";
#endif
    createWordsFromLine(otherVersion, textX, textY+=textYSpacing, &textFont);
    
    textY+=textYBreak;
    
    createWordsFromLine("More of Andy's work at itch.io/andymakes.", textX, textY+=textYSpacing, &textFont);
    createWordsFromLine("More of Dan Friel's work at danfriel.com.", textX, textY+=textYSpacing, &textFont);
    
    textY+=textYBreak;
    
    createWordsFromLine("Logo by Chris Hernandez. Maximilian library by micknoise", textX, textY+=textYSpacing, &textFont);
    createWordsFromLine("Created in NYC using openFrameworks.", textX, textY+=textYSpacing, &textFont);
}

void AboutScreen::setupStepMode(){
    box.width = 600 * ipadAdjust;
    box.height = 350 * ipadAdjust;
    box.x = ofGetWidth()/2 - box.width/2;
    box.y = ofGetHeight()/2 - box.height/2;
    
    float textX = box.x + 22*ipadAdjust;
    float textY = box.y + 40*ipadAdjust;
    float textYSpacing = 40*ipadAdjust;
    float textYBreak = 25*ipadAdjust;
    
    createWordsFromLine("In Step Mode, you select your sound from the top.", textX, textY+=textYSpacing, &textFont);
    
    createWordsFromLine(verb+" on the bottom will add the sound to that", textX, textY+=textYSpacing, &textFont);
    
    createWordsFromLine("time in the sequence.", textX, textY+=textYSpacing, &textFont);
    
    textY+=textYSpacing*0.5;
    
    createWordsFromLine("Step Mode is a little more complex than Normal", textX, textY+=textYSpacing, &textFont);
    
    createWordsFromLine("Mode but gives you better control over your song.", textX, textY+=textYSpacing, &textFont);
    
}

void AboutScreen::turnOn(){
    isActive = true;
    
    timer = 0;
    
    for (int i=0; i<words.size(); i++){
        words[i].reset();
    }
    
    float padding = 120;
    for (int i=0; i<bgCircles.size(); i++){
        bgCircles[i].setup(ofRandom(box.x+padding, box.x+box.width-padding), ofRandom(box.y+padding, box.y+box.height-padding), box);
    }
}
void AboutScreen::turnOff(){
    if (!canTurnOff()){
        return;
    }
    
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
    
    timer += deltaTime;
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
    
    //tetsing
//    ofSetColor(255, 0, 0);
//    textFont.drawString("Tap anywhere to dismiss this", box.x+242, box.y+box.height-20);
//    
//    titleFont.drawString("Sequencer", box.x+box.width/2 - titleFont.stringWidth("Sequencer")/2, box.y+60);
    
}

void AboutScreen::createWordsFromLine(string line, float x, float y, ofTrueTypeFont * font){
    
    float spaceW = font->stringWidth("_") * 0.75;
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

bool AboutScreen::canTurnOff(){
    return timer > minTimeOn;
}
