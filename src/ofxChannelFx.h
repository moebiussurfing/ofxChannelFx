//
// Created by moebiussurfing on 2019-12-20.
//

#pragma once
#include "ofMain.h"

//----


// 1. to enable ofxPresetsManager. (must add the addon to the project)
//#define INCLUDE_ofxPresetsManager	

// 2. you can choice one of the two implemented gui's:
//#define INCLUDE_ofxGui // simpler gui
#define INCLUDE_ofxGuiExtended2 // better gui

// 3. to include some extra fx's: delay and echotrace
#define INCLUDE_FX_DELAYS	


//----

///	
///	TODO:
///	
///	+	add another paramGroup and move there all settings that must be recalled but not required on gui
///	+	extra fx: gpu lut?
///	+	reduce callbacks
///	

//----

//fx shaders
#include "ofxDotFrag.h"

//gui1
#ifdef INCLUDE_ofxGui
#include "ofxGui.h"
#include "ofxSurfing_ofxGui.h"
#endif

//gui2
#ifdef INCLUDE_ofxGuiExtended2
#include "ofxGuiExtended2.h"
#endif

//optional presets manager
#ifdef INCLUDE_ofxPresetsManager
#include "ofxPresetsManager.h"
#endif

#include "ofxSurfingHelpers.h"

//-

class ofxChannelFx
{

public:
	// main parameters. usualy to use outside the class as: control & preset
	ofParameterGroup parameters;
	ofParameterGroup getParameters() {
		return parameters;
	}

	//--

public:
	ofxChannelFx() {
		//settings folder
		path_GLOBAL_Folder = "ofxChannelFx";

		//TODO: not required
		path_fileName_Session = "ofxChannelFx_Session.xml";

#ifndef INCLUDE_ofxPresetsManager
		path_fileName_Preset = "ofxChannelFx_Preset.xml";//not used when using presetsManager
#endif

		ENABLE_FxChain.set("ENABLE", true);

		vflip = false;
	};

	~ofxChannelFx()
	{
		exit();
	};

	//--

private:
	void setup_FxChannel();
	void update_FxChannel();

	void setup_GuiTheme();
	void refresh_Gui();
	void refresh_ofxGuiExtended_Check();//check if no fx enabled, then collapse all gui panels
	void refresh_ofxGui_minimize(bool bUseSolo = false);
	void refreshi_ofxGuiExtended_Minimize();

	//--

	//feed the fx processor
public:
	void begin();
	void end();

	ofParameter<bool> ENABLE_FxChain;//main enabler toggle

private:
	//ofParameter<bool> SHOW_DOT_FX;
	ofParameter<int> SELECT_Fx{ "SELECT FX", 1, 1, 3 };	//select the fx to edit/show gui panel
	ofParameter<std::string> SELECT_Fx_Name{ "FX","" };		//fx name
	ofParameter<bool> SELECT_Solo{ "SOLO", false };		//mute the other fx
	ofParameter<bool> RESET{ "RESET", false };			//reset selected fx
	ofParameter<bool> bHeader{ "HEADER", false };
	ofParameter<bool> SHOW_Gui{ "SHOW GUI", false };
	ofParameter<bool> SHOW_Presets{ "SHOW PRESETS", true };
	ofParameter<bool> ENABLE_Keys{ "ENABLE KEYS", true };
	ofParameter<bool> bMinimize{ "MINIMIZE", false };
	bool bEnableGuiWorkflow = false;

	ofParameter<bool> ENABLE_Monochrome;
	ofParameter<bool> ENABLE_ThreeTones;
	ofParameter<bool> ENABLE_HSB;
#ifdef INCLUDE_FX_DELAYS
	ofParameter<bool> ENABLE_Delay;
	ofParameter<bool> ENABLE_Echotrace;
#endif

	ofParameterGroup params_Session;
	ofParameterGroup params_Subcontrol;
	ofParameterGroup params_Editor;
	ofParameterGroup params_Preset;
	ofParameterGroup params_Control;

	//callbacks
	void Changed_params_Control(ofAbstractParameter &e);
	bool DISABLE_Callbacks;

	//--

private:
	ofFbo fbo_FxChain;
	bool vflip;
public:
	void setVflip(bool b) {
		vflip = b;
	}

	//dot fx shaders
private:
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
	void draw();
	void exit();
	void windowResized(int w, int h);

	ofParameter<int> window_W, window_H, window_X, window_Y;

	void drawGui();

private:

	void fboAllocate();

	void startup();

public:
	//--------------------------------------------------------------
	void loadSettings() {
#ifndef INCLUDE_ofxPresetsManager
		ofxSurfingHelpers::loadGroup(params_Preset, path_GLOBAL_Folder + "/" + path_fileName_Preset);
#endif
	}

	//gui
#ifdef INCLUDE_ofxGuiExtended2
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
#endif

	//--

#ifdef INCLUDE_ofxGui
	ofxPanel gui;
#endif

	//--

	//presetsManager
#ifdef INCLUDE_ofxPresetsManager
public:
	ofxPresetsManager presetsManager;
private:
	void setup_PresetsManager();
	ofParameterGroup params_PresetsManagerTools{ "> PRESETS" };
#endif

#ifdef INCLUDE_ofxGuiExtended2
	ofxGuiGroup2* gui_FxPresets;
#endif

	//--

	//settings
private:
	std::string path_GLOBAL_Folder;
	std::string path_fileName_Session;

#ifndef INCLUDE_ofxPresetsManager
	std::string path_fileName_Preset;
#endif

private:
	ofParameter<glm::vec2> position_Gui;

	//----

public:

	//--------------------------------------------------------------
	void setPath_GlobalFolder(std::string folder)
	{
		ofLogNotice(__FUNCTION__) << folder;
		path_GLOBAL_Folder = folder;
		ofxSurfingHelpers::CheckFolder(folder);
	}
	//--------------------------------------------------------------
	glm::vec2 getGuiPosition()
	{
#ifdef INCLUDE_ofxGuiExtended2
		position_Gui = glm::vec2(guiPanel->getPosition().x, guiPanel->getPosition().y);
#endif

#ifdef INCLUDE_ofxGui
		position_Gui = glm::vec2(gui.getPosition().x, gui.getPosition().y);
#endif

		return position_Gui.get();
	}
	//--------------------------------------------------------------
	void setGuiPosition(int x, int y) {
		setGuiPosition(glm::vec2(x, y));
	}
	//--------------------------------------------------------------
	void setGuiPosition(glm::vec2 pos) {
		position_Gui = pos;

#ifdef INCLUDE_ofxGuiExtended2
		guiPanel->setPosition(position_Gui.get().x, position_Gui.get().y);
#endif

#ifdef INCLUDE_ofxGui
		gui.setPosition(position_Gui.get().x, position_Gui.get().y);
#endif
	}
	//--------------------------------------------------------------
	float getGuiWidth()
	{
		float _gwidth;

#ifdef INCLUDE_ofxGuiExtended2
		_gwidth = guiPanel->getWidth();
#endif
#ifdef INCLUDE_ofxGui
		_gwidth = gui.getShape().getWidth();
#endif
		return _gwidth;
	}
	//--------------------------------------------------------------
	void setVisibleGui(bool b) {
		SHOW_Gui = b;

#ifdef INCLUDE_ofxGuiExtended2
		guiPanel->getVisible().set(b);
#endif

#ifdef INCLUDE_ofxGui
#endif

#ifdef INCLUDE_ofxPresetsManager
		if (SHOW_Presets && SHOW_Gui) setVisible_PresetClicker(true);
		else if (!SHOW_Gui) setVisible_PresetClicker(false);
#endif
	}

#ifdef INCLUDE_ofxPresetsManager
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
#endif

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

	//-

public:
	//--------------------------------------------------------------
	void setEnableKeys(bool b) {
		ENABLE_Keys = b;
	}
	void keyPressed(int key);

	//-

#ifdef INCLUDE_ofxGuiExtended2
private:
	std::string path_Theme;

public:
	//--------------------------------------------------------------
	void loadTheme(std::string _path) {
		ofLogNotice(__FUNCTION__) << " : " << path_Theme;
		path_Theme = _path;
		guiPanel->loadTheme(path_Theme);
	}
#endif
};
