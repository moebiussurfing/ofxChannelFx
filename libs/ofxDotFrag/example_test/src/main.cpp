
//#include "ofMain.h"
//#include "ofApp.h"
//
////========================================================================
//int main(int argc, char *argv[]){
//
//#ifdef __ARM_ARCH
//    ofGLESWindowSettings settings;
//    settings.glesVersion = 2;
//    settings.setSize( 640, 700 );
//    ofCreateWindow(settings);
//#else        
//    ofSetupOpenGL(640, 700, OF_WINDOW);	        
//#endif
//
//	ofApp *app = new ofApp();
//	app->arguments = vector<string>(argv, argv + argc);
//    ofRunApp( app );
//
//}

#include "ofMain.h"
#include "ofApp.h"
int main() {
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:

	//ofSetupOpenGL(1440, 900, OF_WINDOW);
	ofSetupOpenGL(1920, 1080, OF_WINDOW);

	ofRunApp(new ofApp());
}

//#include "ofMain.h"
//#include "ofApp.h"
//
////========================================================================
//int main() {
//
//	ofGLFWWindowSettings settings;
//
//	settings.setGLVersion(2, 1);  // Fixed pipeline
//
//	// Programmable pipeline >> you need to define GL_VERSION_3 in you pre-processor macros!
//	//settings.setGLVersion(3, 2);  
//	//settings.stencilBits = 8;
//	//settings.numSamples = 4;
//
//	int w = 1300;
//	int h = 1050;
//	settings.setSize(w, h);
//
//	ofCreateWindow(settings);
//	ofRunApp(new ofApp());
//
//}
