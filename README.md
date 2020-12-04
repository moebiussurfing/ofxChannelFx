ofxChannelFx
=============================

# Overview
**ofxChannelFx** is an **openFrameworks** addon to image processing with common effects: HSB, contrast, monochrome, tree tones colors and delays.
Powered by **ofxDotFrag**.

## Screenshot
![image](/readme_images/Capture1.PNG?raw=true "image")

## Features
- Channel strip to do image processing with usualy useful effects:
* Monochrome, Three Tones, Hue/Saturation/Brightness/Contrast, Delay and Echo Trace effects.
- Powered by the shaders of **ofxDotFrag**.
- Store/Recall settings.
- Implemented **ofxGui** or **ofxGuiExtended** GUI's workflow and **ofxPresetsManager**.

## Usage
 
### ofApp.h
```.cpp
#include "ofxChannelFx.h"
ofxChannelFx channelFx;
```

### ofApp.cpp
```.cpp
ofApp::setup(){
	channelFx.setup();
}

ofApp::update(){
	channelFx.begin();
	{
		// draw here
	}
	channelFx.end();
}

ofApp::draw(){
	channelFx.draw();
	channelFx.drawGui();
}
```

## Dependencies
**Already included into ADDON/libs**  
ofxDotFrag (https://github.com/npisanti/ofxDotFrag)

**Optional**  
ofxGui  
ofxGuiExtended2 (https://github.com/moebiussurfing/ofxGuiExtended2)  
ofxPresetsManager (https://github.com/moebiussurfing/ofxPresetsManager)  

## Tested systems
- **Windows10** / **VS2017** / **OF ~0.11**
- **macOS High Sierra** / **Xcode 9/10** / **OF ~0.11**

## Author
Addon by **@moebiusSurfing**  
*(ManuMolina). 2020.*

## License
*MIT License.*