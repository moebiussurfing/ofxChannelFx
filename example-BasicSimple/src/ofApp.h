#pragma once

#include "ofMain.h"

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
