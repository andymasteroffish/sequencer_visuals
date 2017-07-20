#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	sequencer.setup();

	//for maxim
	ofSoundStreamSetup(2, 0, this, sequencer.sampleRate, sequencer.bufferSize, 4);

}


//--------------------------------------------------------------
void ofApp::update() {
	//cout<<"--FRAME "<<ofGetElapsedTimeMillis()<<" --"<<endl;

	sequencer.update();

}

//--------------------------------------------------------------
void ofApp::draw() {

	sequencer.draw();

	if (sequencer.takeScreenshot) {
		sequencer.takeScreenshot = false;
		ofImage pic;
		pic.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		pic.save("screenshots/img" + ofToString(ofGetSystemTime()) + ".png");
	}

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	sequencer.touchDown(x, y);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	sequencer.keyPressed(key);

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	sequencer.mouseMoved(x, y);
}



//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

	sequencer.windowResized(w, h);

}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
	for (int i = 0; i < bufferSize; i++) {
		sequencer.setMaximAudio(i % 2 == 0);
		output[i*nChannels] = sequencer.sampleOut;
		output[i*nChannels + 1] = sequencer.sampleOut;
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
	cout << "haha wut up dude" << endl;
	ofSoundStreamClose();
}