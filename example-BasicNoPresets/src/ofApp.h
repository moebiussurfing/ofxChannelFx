#pragma once

#include "ofMain.h"

// NOTE ABOUT THIS EXAMPLE:
//
// to run this example out-of-the-box you must disable the presets manager addon
// commenting this line on ofxChannelFx.h 
//#define INCLUDE_ofxPresetsManager		// presets manager
//
// you can choice one of the two implemented gui's:
//#define INCLUDE_ofxGui	// simpler gui
//#define INCLUDE_ofxGuiExtended2	// better gui

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
