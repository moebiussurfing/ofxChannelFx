#pragma once

#include "ofMain.h"
#include "ofxChannelFx.h"
#include "ofxGui.h"
#define USE_ofxWindowApp
#ifdef USE_ofxWindowApp
#include "ofxWindowApp.h"
#endif

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void drawCam();
		//void draw3D();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxChannelFx channelFx;
		ofParameter<bool> bEnableFx;

		ofVideoGrabber vidGrabber;//webcam
		ofxPanel gui;
#ifdef USE_ofxWindowApp
		ofxWindowApp windowApp;
#endif
};
