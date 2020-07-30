//
// Created by moebiussurfing on 2019-12-20.
//

///	
///	TODO:
///	
///	+	add another paramGroup and move there all settings that must be recalled but not required on gui
///	+	extra fx: gpu lut?
///	+	reduce callbacks
///	
///	BUG:
///	
///	+	when changing preset gui refreshes bad.
///	


#pragma once
#include "ofMain.h"


//----
//
#define INCLUDE_FX_DELAYS			//extra fx
#define INCLUDE_ofxPresetsManager
//
//----


//fx shaders
#include "ofxDotFrag.h"

//gui
//#define INCLUDE_ofxGuiExtended2
//#ifdef INCLUDE_ofxGuiExtended2
#include "ofxGuiExtended2.h"
//#endif
//TODO: could add simpler ofxGui maybe..

//optional presets manager
#ifdef INCLUDE_ofxPresetsManager
#include "ofxPresetsManager.h"
#endif

class ofxChannelFx
{

public:
	ofxChannelFx() {
		//settings folder
		path_GLOBAL_Folder = "ofxChannelFx";
		path_fileName_Session = "ofxChannelFx_Session.xml";
		path_fileName_Preset = "ofxChannelFx_Preset.xml";//not used when using presetsManager
	};

	~ofxChannelFx()
	{
		//exit();
	};

private:
	ofParameter<glm::vec2> gPos;

	//----

	//API

public:

	//--------------------------------------------------------------
	void setPath_GlobalFolder(string folder)
	{
		ofLogNotice(__FUNCTION__) << folder;
		path_GLOBAL_Folder = folder;
		CheckFolder(folder);
	}
	//--------------------------------------------------------------
	glm::vec2 getGuiPosition()
	{
		gPos = glm::vec2(guiPanel->getPosition().x, guiPanel->getPosition().y);
		return gPos.get();
	}
	//--------------------------------------------------------------
	void setGuiPosition(glm::vec2 pos) {
		gPos = pos;
		guiPanel->setPosition(gPos.get().x, gPos.get().y);
	}
	//--------------------------------------------------------------
	float getGuiWidth()
	{
		float _gwidth = guiPanel->getWidth();
		return _gwidth;
	}
	//--------------------------------------------------------------
	void setVisibleGui(bool b) {
		SHOW_Gui = b;
		guiPanel->getVisible().set(b);

		if(SHOW_Presets && SHOW_Gui) setVisible_PresetClicker(true);
		
		else if(!SHOW_Gui) setVisible_PresetClicker(false);
	}
	//--------------------------------------------------------------
	void setPosition_PresetClicker(int x, int y, int _cellSize)
	{
		presetsManager.setPosition_PresetClicker(x, y, _cellSize);
	}
	//--------------------------------------------------------------
	void setVisible_PresetClicker(bool b)
	{
		presetsManager.setVisible_PresetClicker(b);
	}
	//--------------------------------------------------------------
	void setKeysEnable(bool b) {
		ENABLE_Keys = b;
	}
	//--------------------------------------------------------------
	void doReset() {
		RESET = true;
	}

	ofParameterGroup params_ControlExternal;
	//--------------------------------------------------------------
	ofParameterGroup getParamGroup_Control() {
		return params_ControlExternal;
	}
	
	//bool ENABLE_Active = true;
	//void setActive(bool b);
	//void setGuiVisible(bool b);
	//
	//	ofParameter<bool> SHOW_gui = true;//independent to autohide state
	//	ofEventListener listener_SHOW_gui;
	//	void Changed_SHOW_gui();
	//
	//	ofParameter<int> MODE_App;
	//	ofEventListener listener_MODE_App;
	//	void Changed_MODE_App();
	//
	//public:
	//
	//	void loadPreset(int p)
	//	{
	//		//cout << "loadPreset:" << p << endl;
	//		presetsManager.loadPreset(p);
	//	}
	//	void setUserVisible(bool b)
	//	{
	//		//presets
	//		presetsManager.setVisible_PresetClicker(b);
	//		presetsManager.setEnableKeys(b);
	//	}
	//
	//	void setMODE_App(int m)
	//	{
	//		MODE_App = m;
	//	}

	//--

private:
	void setup_FxChannel();
	void update_FxChannel();

	void setup_GuiTheme();
	void refreshGui_FxChannel();
	void refreshGuiCollapse_FxChannel();//check if no fx enabled, then collapse all gui panels

	//--

	//API

	//bool ENABLE_Keys = false;
public:
	void setEnableKeys(bool b) {
		ENABLE_Keys = b;
	}
	void keyPressed(int key);

	//--

	//feed the fx processor
public:
	void begin();
	void end();

	ofParameter<bool> ENABLE_FxChain;//main enabler toggle

private:
	//ofParameter<bool> SHOW_DOT_FX;
	ofParameter<int> SELECT_Fx{ "SELECT FX", 0, 0, 3 };	//select the fx to edit/show gui panel
	ofParameter<string> SELECT_Fx_Name{ "FX","" };		//fx name
	ofParameter<bool> SELECT_Solo{ "SOLO", false };		//mute the other fx
	ofParameter<bool> RESET{ "RESET", false };			//reset selected fx
	ofParameter<bool> bHeader{ "HEADER", false };
	ofParameter<bool> bMinimize{ "MINIMIZE", false };
	ofParameter<bool> SHOW_Gui{ "SHOW GUI", false };
	ofParameter<bool> SHOW_Presets{ "SHOW PRESETS", true };
	ofParameter<bool> ENABLE_Keys{ "ENABLE KEYS", true };
	//ofParameter<void> bMinimize{ "MINIMIZE" };
	bool bEnableGuiWorkflow = false;

	ofParameter<bool> ENABLE_Monochrome;
	ofParameter<bool> ENABLE_ThreeTones;
	ofParameter<bool> ENABLE_HSB;
#ifdef INCLUDE_FX_DELAYS
	ofParameter<bool> ENABLE_Delay;
	ofParameter<bool> ENABLE_Echotrace;
#endif

	ofParameterGroup params_Session;
	ofParameterGroup params_Editor;
	ofParameterGroup params_Preset;
	ofParameterGroup params_Control;

	//callbacks
	void Changed_params_Control(ofAbstractParameter &e);
	bool DISABLE_Callbacks;

	//--

private:
	ofFbo fbo_FxChain;

	bool bArbPRE;//to debug and avoid conflict with other addons or fbo modes...

	//basic fx
	ofx::dotfrag::Monochrome frag1;
	ofx::dotfrag::ThreeTones frag2;
	ofx::dotfrag::HSB frag3;

	//fx extra
#ifdef INCLUDE_FX_DELAYS
	ofx::dotfrag::Delay frag4;
	ofx::dotfrag::EchoTrace frag5;
	//ofx::dotfrag::Twist frag6;
#endif

	//--

public:
	void setup();
	//void update();
	void draw();
	void exit();
	void windowResized(int w, int h);

	ofParameter<int> window_W, window_H, window_X, window_Y;

	//void drawGui();

private:
	void startup();

	////labels
	////ofTrueTypeFont myFont;
	//string myTTF;// gui font for all gui theme
	//int sizeTTF;
	//string fname;

	//gui
private:
	ofxGui gui;
	ofxGuiPanel *guiPanel;
	ofxGuiGroup2 *guiGroup;
	ofxGuiGroup2 *gui_FxUser;
	ofxGuiGroup2 *gui_FxEdit;

	//basic fx
	ofxGuiGroup2 *gFrag1;
	ofxGuiGroup2 *gFrag2;
	ofxGuiGroup2 *gFrag3;
	//extra fx
#ifdef INCLUDE_FX_DELAYS
	ofxGuiGroup2 *gFrag4;
	ofxGuiGroup2 *gFrag5;
	//ofxGuiGroup2 *gFrag6;
#endif

	//customize
	ofJson j_ButtonBig, j_SliderBig;

	//--

	//simpler gui
	//ofxGui
	//ofxPanel gui_FxUser;

	//--

	//presetsManager
#ifdef INCLUDE_ofxPresetsManager
public:
	ofxPresetsManager presetsManager;
private:
	void setup_PresetsManager();
	ofParameterGroup params_PresetsManagerTools{ "> PRESETS" };
	ofxGuiGroup2* gui_FxPresets;
#endif

	//--

	//settings
private:
	string path_GLOBAL_Folder;
	string path_fileName_Preset;
	string path_fileName_Session;

	void saveGroup(ofParameterGroup &g, string path);
	void loadGroup(ofParameterGroup &g, string path);

private:
	//--------------------------------------------------------------
	void CheckFolder(string _path)
	{
		ofLogNotice(__FUNCTION__) << _path;

		ofDirectory dataDirectory(ofToDataPath(_path, true));

		//check if folder path exist
		if (!dataDirectory.isDirectory())
		{
			ofLogError(__FUNCTION__) << "FOLDER NOT FOUND! TRYING TO CREATE...";

			//try to create folder
			bool b = dataDirectory.createDirectory(ofToDataPath(_path, true));

			//debug if creation has been succeded
			if (b) ofLogNotice(__FUNCTION__) << "CREATED '" << _path << "'  SUCCESSFULLY!";
			else ofLogError(__FUNCTION__) << "UNABLE TO CREATE '" << _path << "' FOLDER!";
		}
		else
		{
			ofLogNotice(__FUNCTION__) << "OK! LOCATED FOLDER: '" << _path << "'";//nothing to do
		}
	}
};
