#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
#ifdef USE_ofxWindowApp
	windowApp.setFrameRate(60);
	windowApp.setVerticalSync(true);
#endif

	font.load(OF_TTF_MONO, 10);

	//--


	//prims
	
	//box
	float _size = 100;
	box.set(_size);
	box.setPosition(0, _size * 0.5, 0);
	box.setResolution(1);

	//cone
	float _cSize = 1.62 * _size;
	cone.set(_size, _cSize, 3, 1, 1);
	cone.setPosition(0, _cSize * 0.5, 0);
	cone.tiltDeg(180);

	//cam.enableInertia();
	cam.enableOrtho();
	cam.setFarClip(10000);
	cam.setNearClip(-10000);

	//--

	////material
	//if (bUseMaterial) {
	//	//ofEnableLighting();
	//	//ofEnableDepthTest();
	//	ofSetSmoothLighting(true);
	//	light.setup();
	//	light.enable();
	//	light.setSpotlight();
	//	light.setSpotlightCutOff(50);
	//	//light.setPosition(0, 100, 100);

	//	material.setDiffuseColor(ofColor::red);
	//	material.setAmbientColor(ofColor::red);
	//	material.setSpecularColor(ofColor::white);
	//	material.setShininess(128);
	//}

	//--

	//webcam
	setupWebcamDevice();

	//--

	//ofxChannelFx
	channelFx.setup();
	channelFx.setGuiPosition(glm::vec2(ofGetWidth() - 230, 10));
	channelFx.setEnableKeys(true);
}

//--------------------------------------------------------------
void ofApp::update() {
	vidGrabber.update();

	//-

	//ofxChannelFx
	//feed
	channelFx.begin();
	{
		drawScene();
	}
	channelFx.end();
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetBackgroundColor(0);

	//--

	//ofxChannelFx
	//draw processed
	channelFx.draw();

	//--

	drawWebcamInfo();
}

//--------------------------------------------------------------
void ofApp::drawScene() {

	drawWebcam();

	draw3D();
}

//webcam
//--------------------------------------------------------------
void ofApp::setupWebcamDevice() {

	//load settings
	ofXml _xml;
	bool _isLoaded;
	_isLoaded = _xml.load("settings.xml");
	ofDeserialize(_xml, _dName);
	ofLogNotice(__FUNCTION__) << _xml.toString();

	//start
	auto _devs = vidGrabber.listDevices();
	_d = -1;
	if (_isLoaded) {
		for (int i = 0; i < _devs.size(); i++) {
			if (_devs[i].deviceName == _dName.get()) {
				_d = i;
				ofLogNotice(__FUNCTION__) << "device name:\t" << _dName.get();
				ofLogNotice(__FUNCTION__) << "device index:\t" << _d;
			}
		}
	}
	if (_d == -1) {//error. try to load first device...
		_d = 0;//select cam device
		_dName = _devs[_d].deviceName;
	}

	//start device
	vidGrabber.setVerbose(true);
	vidGrabber.setDeviceID(_d);
	vidGrabber.setup(1920, 1080);
}
//--------------------------------------------------------------
void ofApp::drawWebcamInfo() {
	//display device name
	string str;
	str += "[" + ofToString(_d) + "] " + _dName.get();
	str += " " + ofToString(vidGrabber.isInitialized() ? "ON" : "OFF");
	str += "\ni: select next device";
	str += "\nI: restart device";

	if (!font.isLoaded()) ofDrawBitmapStringHighlight(str, 30, 20);
	else {
		ofPushStyle();
		//bbox
		ofSetColor(0, 150);
		ofFill();
		ofRectangle _r(font.getStringBoundingBox(str, 30, 20));
		int pad = 5;
		_r.setWidth(_r.getWidth() + pad);
		_r.setHeight(_r.getHeight() + pad);
		_r.setX(_r.getPosition().x - pad / 2.);
		_r.setY(_r.getPosition().y - pad / 2.);
		//_r.scaleFromCenter(1.05, 1.5);
		ofDrawRectangle(_r);
		//text
		ofSetColor(255);
		ofNoFill();
		font.drawString(str, 30, 20);
		ofPopStyle();
	}
}
//--------------------------------------------------------------
void ofApp::drawWebcam() {
	ofRectangle r(0, 0, vidGrabber.getWidth(), vidGrabber.getHeight());
	r.scaleTo(ofGetWindowRect(), OF_SCALEMODE_CENTER);
	//r.scaleTo(ofGetWindowRect(), OF_SCALEMODE_STRETCH_TO_FILL);
	//r.scaleTo(ofGetWindowRect(), OF_SCALEMODE_FILL);
	vidGrabber.draw(r.x, r.y, r.width, r.height);
}
//--------------------------------------------------------------
void ofApp::exitWebcam() {
	ofXml _xml;
	ofSerialize(_xml, _dName);
	_xml.save("settings.xml");
}

//--------------------------------------------------------------
void ofApp::draw3D() {
	ofEnableDepthTest();
	//ofEnableAlphaBlending();

	ofPushStyle();

	cam.begin();
	{
		//-

		int _speed = 60 * 10;
		ofRotateYDeg(ofMap(ofGetFrameNum() % _speed, 0, _speed, 0, 359));

		//-

		//if (bUseMaterial) {
		//	//ofEnableDepthTest();

		//	ofEnableLighting();
		//	//light.setPosition(cam.getPosition().x, cam.getPosition().y, cam.getPosition().z);
		//	light.setPosition(-200, 200, 200);
		//	light.enable();

		//	//material.begin();
		//}
		//else {
		//	//ofDisableDepthTest();
		//}

		//-

		//prims
		ofFill();
		ofSetColor(0, 64);
		//ofSetColor(255, 64);
		//ofSetColor(0);
		if (_prim == 0) {
			box.drawFaces();
		}
		else if (_prim == 1) {
			cone.drawFaces();
		}

		//-

		//if (bUseMaterial) {
		//	//material.end();

		//	light.disable();
		//	ofDisableLighting();

		//	//ofDisableDepthTest();
		//}

		//-

		//wire prims
		ofNoFill();
		ofSetColor(255, 200);
		//ofSetColor(0, 200);
		//ofSetColor(255);
		ofSetLineWidth(2.);
		if (_prim == 0) {
			box.drawWireframe();
		}
		else if (_prim == 1) {
			cone.drawWireframe();
		}
	}
	cam.end();

	ofPopStyle();

	//ofDisableAlphaBlending();
	ofDisableDepthTest();
}

//--------------------------------------------------------------
void ofApp::exit()
{
	exitWebcam();

	//ofxChannelFx
	channelFx.exit();

#ifdef USE_ofxWindowApp
	windowApp.exit();
#endif
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	//ofxChannelFx
	channelFx.keyPressed(key);

	//-

	if (key == ' ') {
		_prim++; if (_prim == 2) _prim = 0;
	}

	//webcam
	if (key == 'i') {
		//webcam
		auto _devs = vidGrabber.listDevices();
		_d++;
		if (_d > _devs.size() - 1) _d = 0;
		_dName = _devs[_d].deviceName;

		//select cam device
		vidGrabber.close();
		vidGrabber.setDeviceID(_d);
		vidGrabber.setup(1920, 1080);
	}
	if (key == 'I') {
		//restart device
		//if (vidGrabber.isInitialized()) {
		//	vidGrabber.close();
		//}
		//else 
		{
			vidGrabber.close();
			vidGrabber.setDeviceID(_d);
			vidGrabber.setup(1920, 1080);
			//vidGrabber.initGrabber(1920, 1080);
		}
	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}
