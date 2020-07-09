#pragma once

#include "ofMain.h"

#include "ofxChannelFx.h"

#define USE_ofxWindowApp
#ifdef USE_ofxWindowApp
#include "ofxWindowApp.h"
#endif

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();
		void keyPressed(int key);
		void windowResized(int w, int h);

		ofxChannelFx channelFx;

		//draw
		void drawScene();

		//webcam
		void drawWebcam();
		ofVideoGrabber vidGrabber;
		
		//3D scene
		void draw3D();
		ofEasyCam cam;
		
#ifdef USE_ofxWindowApp
		ofxWindowApp windowApp;
#endif
};
