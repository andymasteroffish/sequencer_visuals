#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    publicRelease = false;
    
    usingFFT = false;
    useNumpadKeys = false;
    usePreHitDetection = true;
    
    autoPlay = false;
    recording = true;
    turnOnRecordingWhenClearing = true;
    
    ofSetFrameRate(60);
    
    whiteVal = 240;
    ofBackground(whiteVal);
    
    showHelp = false;
    
    bpm.reset();
    bpmStartValue = 160;
    bpmValue = bpmStartValue;
    bpm.setBpm(bpmValue);
    bpm.setBeatPerBar(8);
    bpm.prcForPreHit = 0.9;
    ofAddListener(bpm.beatEvent, this, &ofApp::hitBeat);
    ofAddListener(bpm.preBeatEvent, this, &ofApp::preHitBeat);
    bpm.start();
    
    thisBeat = 0;
    onPreHit = false;
    
    if (usingFFT){
        fft.setup(NUM_BANDS);
        showFFT = false;
    }
    

    prevFrameTime = ofGetElapsedTimef();
    deltaTime = 0;
    
    timeSinceLastHit = 0;
    minTimeForNextHit = 0.03;
    
    
    //randomzie hit IDs on start (for FFT)
    for (int i=0; i<15; i++){
        hitIDs[i] = i+1;
    }
    for (int i=0; i<1000; i++){
        int a = (int)ofRandom(15);
        int b = (int)ofRandom(15);
        int temp = hitIDs[a];
        hitIDs[a] = hitIDs[b];
        hitIDs[b] = temp;
    }
    
    //set the sounds
    loadSounds("../sound_source.txt");
    
    clearBeats();
    
    //set the markers
    beatXSpacing = 40;
    beatYDistFromBottom = 40;
    float beatXPadding = (ofGetWidth()-(beatXSpacing*NUM_BEATS))/2;
    for (int i=0; i<NUM_BEATS; i++){
        beatMarkers[i].setup(beatXPadding+beatXSpacing*i, ofGetHeight()-beatYDistFromBottom);
    }
    
}

//--------------------------------------------------------------
void ofApp::preHitBeat(void){
    //cout<<"pre hit!!"<<endl;
    if (usePreHitDetection){
        onPreHit = true;
    }
}
//--------------------------------------------------------------
void ofApp::hitBeat(void){
    onPreHit = false;
    thisBeat++;
    if (thisBeat >= NUM_BEATS){
        thisBeat = 0;
    }
    
    //play anything that's on
    for (int k=0; k<NUM_SOUNDS; k++){
        if (beatsOn[thisBeat][k]){
            makeNewHit(k);
        }
    }
    
    beatMarkers[thisBeat].triggerBeat();
    
    if (autoPlay && ofRandomuf() < 0.2){
        makeNewHit(ofRandom(15));
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    deltaTime = ofGetElapsedTimef() - prevFrameTime;
    prevFrameTime = ofGetElapsedTimef();
    
    timeSinceLastHit += deltaTime;
    
    if (usingFFT){
        fft.update();
        
        //check if the FFT has a new hit for is
        if (fft.haveNewHit){
            makeNewHit(fft.bandsOn);
        }
    }
    
    for (int i=hits.size()-1; i>=0; i--){
        hits[i]->update(deltaTime);
        if (hits[i]->killMe){
            hits[i]->cleanUp();
            delete hits[i];
            hits.erase(hits.begin()+i);
        }
    }
    
    //update the markers
    for (int i=0; i<NUM_BEATS; i++){
        beatMarkers[i].update(deltaTime);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (showFFT){
        fft.draw();
        ofSetColor(0);
        string debugText = "";
        debugText += "active: "+ofToString(hits.size())+"\n";
        ofDrawBitmapString(debugText, 10, 10);
    }
    

    for (int i=0; i<hits.size(); i++){
        hits[i]->draw();
    }
    
    //draw the markers
    for (int i=0; i<NUM_BEATS; i++){
        bool anyOn = false;
        for (int k=0; k<NUM_SOUNDS; k++){
            if (beatsOn[i][k])  anyOn = true;
        }
        
        beatMarkers[i].draw(anyOn, recording);
    }
    
    if (!publicRelease){
        ofSetColor(0);
        string text =   "Sequencer by Andy Wallace - andy@andymakes.com";
        text +=         "\nWork in progress. Please do not distribute.";
        text +=         "\nPress h for help.";
        ofDrawBitmapString(text, 10, 15);
    }
    
    if (showHelp){
        ofSetColor(0);
        string text = "";
        text += "keys 1-0 & q-t for sounds\n";
        text += "return to clear\n";
        text += "space to start/stop recording\n";
        text += "\n";
        text += "Up & down to change BPM (currently "+ofToString(bpmValue)+")\n";
        text += "Left to reset BPM\n";
        text += "\n";
        text += "f to toggle full screen\n";
        string turnOnRecordStatus = (turnOnRecordingWhenClearing ? "on" : "off");
        text += "c to toggle turning on record when clearing (currently "+turnOnRecordStatus+")\n";
        string preHitStatus = (usePreHitDetection ? "on" : "off");
        text += "p to toggle pre hit detection (currently "+preHitStatus+")\n";
        string autoStatus = (autoPlay ? "on" : "off");
        text += "a for auto play (currently "+autoStatus+")\n";
        text += "h to hide this\n";
        
        ofDrawBitmapString(text, 10, 90);
    }
    
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //cout<<"key "<<key<<endl;
    if (key == 'h' || key == 'H'){
        showHelp = !showHelp;
    }
    if (key == 'a'){
        autoPlay = !autoPlay;
    }
    if (key == 'f'){
        ofToggleFullscreen();
    }
    if (key == 'c'){
        turnOnRecordingWhenClearing = !turnOnRecordingWhenClearing;
    }
    if (key == 'p'){
        usePreHitDetection = !usePreHitDetection;
    }
    
    if (key == ' '){
        recording = !recording;
    }
    
    if (key == OF_KEY_UP){
        bpmValue += 10;
        bpmValue = MIN(bpmValue, 400);
        bpm.setBpm(bpmValue);
    }
    if (key == OF_KEY_DOWN){
        bpmValue -= 10;
        bpmValue = MAX(bpmValue, 50);
        bpm.setBpm(bpmValue);
    }
    if (key == OF_KEY_LEFT){
        bpmValue = bpmStartValue;
        bpm.setBpm(bpmValue);
    }
    
    if (useNumpadKeys){
        if (key == 63289)   makeNewHit(0);  //num lock
        if (key == '/')     makeNewHit(1);
        if (key == '*')     makeNewHit(2);
        if (key == 127)     makeNewHit(3);  //backspace
        
        if (key == '7')     makeNewHit(4);
        if (key == '8')     makeNewHit(5);
        if (key == '9')     makeNewHit(6);
        if (key == '-')     makeNewHit(7);
        
        if (key == '4')     makeNewHit(8);
        if (key == '5')     makeNewHit(9);
        if (key == '6')     makeNewHit(10);
        if (key == '+')     makeNewHit(11);
        
        if (key == '1')     makeNewHit(12);
        if (key == '2')     makeNewHit(13);
        if (key == '3')     makeNewHit(14);
        
        if (key == '0') {
            recording = !recording;
        }
        
        if (key == 13){  //Enter
            clearBeats();
        }
    }
    
    //laptop control
    if (!useNumpadKeys){
        if (key == '0')     makeNewHit(0);
        if (key == '1')     makeNewHit(1);
        if (key == '2')     makeNewHit(2);
        if (key == '3')     makeNewHit(3);
        if (key == '4')     makeNewHit(4);
        if (key == '5')     makeNewHit(5);
        if (key == '6')     makeNewHit(6);
        if (key == '7')     makeNewHit(7);
        if (key == '8')     makeNewHit(8);
        if (key == '9')     makeNewHit(9);
        if (key == 'q')     makeNewHit(10);
        if (key == 'w')     makeNewHit(11);
        if (key == 'e')     makeNewHit(12);
        if (key == 'r')     makeNewHit(13);
        if (key == 't')     makeNewHit(14);
        
        if (key == 13){  //Enter
            clearBeats();
        }
    }
    
    
}


//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    //set the markers
    float beatXPadding = (ofGetWidth()-(beatXSpacing*NUM_BEATS))/2;
    for (int i=0; i<NUM_BEATS; i++){
        beatMarkers[i].pos.set(beatXPadding+beatXSpacing*i, ofGetHeight()-beatYDistFromBottom);
    }
}

//--------------------------------------------------------------
void ofApp::makeNewHit(bool bandsOn[NUM_BANDS]){
    if (timeSinceLastHit < minTimeForNextHit){
        return;
    }
    
    timeSinceLastHit = 0;
    
    //set half bands
    bool bandsOnMicro[NUM_BANDS/2];
    for (int i=0; i<NUM_BANDS; i+=2){
        bandsOnMicro[i/2] = bandsOn[i] || bandsOn[i+1];
    }
    
    //get the total value
    int idNum = 0;
    for (int i=0; i<NUM_BANDS/2; i++){
        if(bandsOnMicro[i]){
            idNum += pow(2,i);
        }
    }
    
    //testing
    //idNum = idNum % 9;
    
    Hit * thisHit;
    
    if (idNum == 0)     thisHit = new TunnelHit();  //THIS CAN NEVER BE 0, BECAUSE AT LEAST ONE BAND MUST BE ON
    
    if (idNum == hitIDs[0])     thisHit = new TunnelHit();
    if (idNum == hitIDs[1])     thisHit = new SweepHit();
    if (idNum == hitIDs[2])     thisHit = new TriangleHit();
    if (idNum == hitIDs[3])     thisHit = new GrapesHit();
    
    if (idNum == hitIDs[4])     thisHit = new BuckshotHit();
    if (idNum == hitIDs[5])     thisHit = new ChaserHit();
    if (idNum == hitIDs[6])     thisHit = new SlashHit();
    if (idNum == hitIDs[7])     thisHit = new SquareHit();
    
    if (idNum == hitIDs[8])     thisHit = new TrapezoidHit();
    if (idNum == hitIDs[9])     thisHit = new DotPolygonHit();
    if (idNum == hitIDs[10])    thisHit = new SizzleHit();
    if (idNum == hitIDs[11])    thisHit = new DrunkTriangleHit();
    
    if (idNum == hitIDs[12])    thisHit = new CrystalHit();
    if (idNum == hitIDs[13])    thisHit = new ClapHit();
    if (idNum == hitIDs[14])    thisHit = new WaveColumnHit();
    
    thisHit->setup(ofGetWidth(), ofGetHeight(), whiteVal);
    
    hits.push_back(thisHit);
}

//--------------------------------------------------------------
void ofApp::makeNewHit(int idNum){
    //assume we'll play the sound
    bool playSound = true;
    
    
    //turning this beat on
    if (recording){
        int beatPos = thisBeat;
        if (onPreHit){
            playSound = false;  //we'll catch it in a second
            beatPos = (thisBeat+1)%NUM_BEATS;
        }
        beatsOn[beatPos][idNum] = true;
    }
    
    //Making the hit
    if (playSound){
        Hit * thisHit;
        
        if (idNum == 0)     thisHit = new TunnelHit();
        if (idNum == 1)     thisHit = new SweepHit();
        if (idNum == 2)     thisHit = new TriangleHit();
        if (idNum == 3)     thisHit = new GrapesHit();
        if (idNum == 4)     thisHit = new BuckshotHit();
        if (idNum == 5)     thisHit = new ChaserHit();
        if (idNum == 6)     thisHit = new SlashHit();
        if (idNum == 7)     thisHit = new SquareHit();
        if (idNum == 8)     thisHit = new TrapezoidHit();
        if (idNum == 9)     thisHit = new DotPolygonHit();
        if (idNum == 10)     thisHit = new SizzleHit();
        if (idNum == 11)     thisHit = new DrunkTriangleHit();
        if (idNum == 12)     thisHit = new CrystalHit();
        if (idNum == 13)     thisHit = new ClapHit();
        if (idNum == 14)     thisHit = new WaveColumnHit();
        
        thisHit->setup(ofGetWidth(), ofGetHeight(), whiteVal);
        
        hits.push_back(thisHit);
    }
    
    //making the sound
    if (playSound){
        sounds[idNum].play();
    }
    
}



//--------------------------------------------------------------
void ofApp::clearBeats(){
    for (int i=0; i<NUM_BEATS; i++){
        for (int k=0; k<NUM_SOUNDS; k++){
            beatsOn[i][k] = false;
        }
        beatMarkers[i].triggerClear();
    }
    if (turnOnRecordingWhenClearing){
        recording = true;
    }
}


//--------------------------------------------------------------
void ofApp::loadSounds(string filePath){
    
    ofBuffer buffer = ofBufferFromFile(filePath);
    vector<string> files;
    
    if (buffer.size() > 0){
        
        for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it){
            
            string line = *it;
            
            if (line.size() > 0){
                if (line[0] != ';'){
                    files.push_back(line);
                }
            }
            
        }
        
        cout<<"found "<<files.size()<<" sound files"<<endl;
        
        for (int i=0; i<NUM_SOUNDS; i++){
            sounds[i].load(files[i%files.size()]);
            sounds[i].setMultiPlay(true);
        }
        
    }else{
        cout<<"bad file"<<endl;
    }
}