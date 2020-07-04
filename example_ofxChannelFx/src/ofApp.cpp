#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
#ifdef USE_ofxWindowApp
	windowApp.setFrameRate(60);
	windowApp.setVerticalSync(true);
#endif

	//webcam
	int _d = 0;
	vidGrabber.setDeviceID(_d);
	vidGrabber.setup(1920,1080);
	//vidGrabber.setup(640, 480);

	bEnableFx.set("FX", true);

	gui.setup();
	gui.add(bEnableFx);
	//gui.add(channelFx.ENABLE_DOT_FX);

	channelFx.setup();
	channelFx.setGuiPosition(glm::vec2(ofGetWidth() - 210, 10));
}

//--------------------------------------------------------------
void ofApp::update() {
	vidGrabber.update();

	if (bEnableFx) {
		channelFx.begin();
	
		drawCam();

		channelFx.end();
	}

}

//--------------------------------------------------------------
void ofApp::draw() {
	
	if (bEnableFx) {
		channelFx.draw();
	}
	else
	{
		drawCam();
	}

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::drawCam() {
	//float _ratioSource = vidGrabber.getWidth() / vidGrabber.getHeight();
	//float _ratioScreen = ofGetWidth() / (float)ofGetHeight();
	//float _ratio = _ratioScreen / _ratioSource;
	//vidGrabber.draw(0, 0, ofGetHeight() * _ratio, ofGetHeight());
	//vidGrabber.draw(0, 0, ofGetWidth(), ofGetHeight());
	{
		ofRectangle r(0, 0, vidGrabber.getWidth(), vidGrabber.getHeight());
		r.scaleTo(ofGetWindowRect());
		vidGrabber.draw(r.x, r.y, r.width, r.height);
	}
}

////--------------------------------------------------------------
//void ofApp::draw3d() {
//
//	int _speed = 60 * 10;
//	ofBoxPrimitive box;
//
//	cam.begin();
//	ofRotateYDeg(ofMap(ofGetFrameNum() % _speed, 0, _speed, 0, 359));
//	box.set(200);
//	ofPushStyle();
//	ofSetColor(0);
//	ofFill();
//	box.drawFaces();;
//	ofNoFill();
//	ofSetColor(255);
//	box.drawWireframe();;
//	ofPopStyle();
//	cam.end();
//}


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
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
