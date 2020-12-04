#pragma once

#include "ofMain.h"

// NOTE:
// to run this example you must disable the presets manager addon
// commenting this line on ofxChannelFx.h 
//#define INCLUDE_ofxPresetsManager		// presets manager

#include "ofxChannelFx.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();
		void keyPressed(int key);
		void windowResized(int w, int h);

		ofxChannelFx channelFx;

		ofImage img;
};
