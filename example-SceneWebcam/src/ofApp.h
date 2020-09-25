#pragma once

#include "ofMain.h"

#include "ofxChannelFx.h"

//#define USE_ofxWindowApp
#ifdef USE_ofxWindowApp
#include "ofxWindowApp.h"
#endif

class ofApp : public ofBaseApp{
	public:
		//webcam
		ofVideoGrabber vidGrabber;
		void setupWebcamDevice();
		void drawWebcamInfo();
		void exitWebcam();
		void drawWebcam();
		ofParameter <std::string> _dName{ "WEBCAM_DEVICE_NAME", "" };
		int _d;

	public:
		void setup();
		void update();
		void draw();
		void exit();
		void keyPressed(int key);
		void windowResized(int w, int h);

		//ofxChannelFx
		ofxChannelFx channelFx;

		//draw
		void setupScene();
		void drawScene();
		
		//3D scene
		void draw3D();
		ofEasyCam cam;
		
#ifdef USE_ofxWindowApp
		ofxWindowApp windowApp;
#endif
		int _prim = 0;
		ofBoxPrimitive box;
		ofConePrimitive cone;

		ofTrueTypeFont font;

		////material
		//bool bUseMaterial = false;
		//ofLight light;
		//ofMaterial material;
};
