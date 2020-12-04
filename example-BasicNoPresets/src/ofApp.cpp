#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(0);
	img.load("image.jpg");

	channelFx.setup();
	channelFx.setVisibleGui(true);
}

//--------------------------------------------------------------
void ofApp::update() {
	channelFx.begin();
	{
		float scale = (ofGetFrameNum() % 600) / 600.f;
		ofScale(ofMap(scale, 0, 1, 1, 1.2));
		img.draw(0, 0, ofGetWidth(), ofGetHeight());
	}
	channelFx.end();
}

//--------------------------------------------------------------
void ofApp::draw() {
	channelFx.draw();
	channelFx.drawGui();
}

//--------------------------------------------------------------
void ofApp::exit() {
	//channelFx.exit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	channelFx.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	channelFx.windowResized(w, h);
}
