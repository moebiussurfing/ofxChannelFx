#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
#ifdef USE_ofxWindowApp
	windowApp.setFrameRate(60);
	windowApp.setVerticalSync(true);
#endif

	//webcam
	int _d = 0;//select cam device
	vidGrabber.setDeviceID(_d);
	vidGrabber.setup(1920, 1080);

	//--

	channelFx.setup();
	channelFx.setGuiPosition(glm::vec2(ofGetWidth() - 230, 10));
}

//--------------------------------------------------------------
void ofApp::update() {
	vidGrabber.update();

	//-

	//feed fx here
	channelFx.begin();
	{
		drawScene();
	}
	channelFx.end();
}

//--------------------------------------------------------------
void ofApp::draw() {

	//draw processed by fx
	channelFx.draw();
}

//--------------------------------------------------------------
void ofApp::drawScene() {
	drawWebcam();
	//draw3D();
}

//--------------------------------------------------------------
void ofApp::drawWebcam() {
	ofRectangle r(0, 0, vidGrabber.getWidth(), vidGrabber.getHeight());
	r.scaleTo(ofGetWindowRect());
	vidGrabber.draw(r.x, r.y, r.width, r.height);
}

//--------------------------------------------------------------
void ofApp::draw3D() {
	ofPushStyle();

	int _speed = 60 * 10;
	ofBoxPrimitive box;

	cam.begin();
	ofRotateYDeg(ofMap(ofGetFrameNum() % _speed, 0, _speed, 0, 359));
	box.set(200);
	ofSetColor(0);
	ofFill();
	box.drawFaces();;
	ofNoFill();
	ofSetColor(255);
	box.drawWireframe();;
	cam.end();

	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::exit()
{
	channelFx.exit();

#ifdef USE_ofxWindowApp
	windowApp.exit();
#endif
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}
