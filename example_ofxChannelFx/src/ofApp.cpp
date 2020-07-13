#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
#ifdef USE_ofxWindowApp
	windowApp.setFrameRate(60);
	windowApp.setVerticalSync(true);
#endif

	//--

	//webcam
	_d = 0;//select cam device
	vidGrabber.setVerbose();
	vidGrabber.setDeviceID(_d);
	vidGrabber.setup(1920, 1080);
	
	auto _devs = vidGrabber.listDevices();
	int _devsAmnt = _devs.size();
	_dName = "[" + ofToString(_d) + "] " + _devs[_d].deviceName;

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

	//--

	//display device name
	string str = _dName;// +"\n" + vidGrabber.
	ofDrawBitmapStringHighlight(str, 30, 20);
}

//--------------------------------------------------------------
void ofApp::drawScene() {
	drawWebcam();
	draw3D();
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
	ofDisableDepthTest();
	ofEnableAlphaBlending();

	int _speed = 60 * 10;

	//ofBoxPrimitive box;
	//box.set(200);
	//box.setResolution(1);

	ofConePrimitive cone;
	cone.set(100, 200, 3, 1, 1);
	cone.tiltDeg(180);

	cam.begin();
	{
		ofRotateYDeg(ofMap(ofGetFrameNum() % _speed, 0, _speed, 0, 359));

		ofNoFill();
		ofSetColor(255, 200);
		//box.drawWireframe();;
		cone.drawWireframe();;

		ofSetColor(0, 64);
		ofFill();
		//box.drawFaces();;
		cone.drawFaces();;
	}
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
	if (key == 'i') {
		auto _devs = vidGrabber.listDevices();
		int _devsAmnt = _devs.size();

		//webcam
		_d++;
		if (_d > _devsAmnt - 1) _d = 0;
		_dName = "[" + ofToString(_d) + "] " + _devs[_d].deviceName;

		//select cam device
		vidGrabber.setDeviceID(_d);
		vidGrabber.setup(1920, 1080);
	}
	if (key == 'I') {
		//restart devices
		vidGrabber.close();
		vidGrabber.setDeviceID(_d);
		vidGrabber.initGrabber(1920, 1080);
		vidGrabber.setup(1920, 1080);
	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}
