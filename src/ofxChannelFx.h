//
// Created by mosbiusurfing on 2019-12-20.
//

#pragma once

//--

//#define INCLUDE_FX_DELAYS

#define INCLUDE_DOT_FRAG
#ifdef INCLUDE_DOT_FRAG
#include "ofxDotFrag.h"
#endif

////--
//
#include "ofMain.h"
#include "ofxGuiExtended2.h"
//#include "ofxPresetsManager.h"
//
class ofxChannelFx
{
//
private:

	string path_GLOBAL_Settings;
	string path_fileName_VideoFX;// = path_GLOBAL_Settings + "videoFX.json";
//
//	bool ENABLE_Active = true;
//	bool ENABLE_Keys = true;

public:
	ofParameter<glm::vec2> gPos;
	glm::vec2 getGuiPosition()
	{
		//glm::vec2 gPos = glm::vec2(panel_ENGINE->getPosition().x, panel_ENGINE->getPosition().y);
		return gPos.get();
	}
	void setGuiPosition(glm::vec2 pos) {
		gPos = pos;
		gui_VideoFx->setPosition(gPos.get().x, gPos.get().y);
	}
	//float getGuiWidth()
	//{
	//	float gwidth = panel_ENGINE->getWidth();
	//	return gwidth;
	//}

public:

	ofxChannelFx() {};
	~ofxChannelFx()
	{
		//exit();
	};
//
//	//----
//
//	//API
//
//public:
//
//	void loadPreset(int p)
//	{
//		//cout << "loadPreset:" << p << endl;
//		presetsManager.loadPreset(p);
//	}
//
//	void setActive(bool b);
//	void setKeysEnable(bool b);
//	void setGuiVisible(bool b);
//
//	//TODO:
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
//
//	//auto hide controlled outside the class. internal by default
//	void setAudoHideExternal(bool b)
//	{
//		ENABLE_AutoHide_external = b;
//	}
//
//	void setAudoHide(bool b)
//	{
//		ENABLE_AutoHide = b;
//		//lastMovement = ofGetSystemTimeMillis();
//	}
//
//	//bool isPlaying_GEN()
//	//{
//	//	return GEN.isPlaying();
//	//}
//
//	void setPlay(bool b)
//	{
//		PLAYING = b;
//	}
//	bool isPlaying()
//	{
//		return PLAYING;
//	}
//
//#ifdef USE_GEN_PLAYER
//	void setPlay_GEN(bool b)
//	{
//		if (b)
//			GEN.play();
//		else
//			GEN.stop();
//	}
//
//	bool isPlaying_GEN()
//	{
//		return GEN.isPlaying();
//	}
//
//	bool get_GEN_EditorMode()
//	{
//		return SHOW_GEN.get();
//	}
//#endif
//
//	//--
//
//private:
//
//	//--
//
#ifdef INCLUDE_DOT_FRAG
	void setup_DotFrag();
	void update_DotFrag();
	void refreshGuiDotFrag();
	
	void begin();
	void end();

	ofParameter<bool> ENABLE_DOT_FX;
	ofParameter<bool> SHOW_DOT_FX;
	ofParameter<int> SELECT_Fx{ "FX", 0, 0, 3 };
	ofParameterGroup params_Control_VideoFX;//only to use callback for fx selector
	ofParameterGroup params_VideoFX;
	void Changed_params_VideoFX(ofAbstractParameter &e);

	//ofxGui
	//ofxPanel gui_VideoFx;

	ofFbo fbo_VideoFx;

	//fx
	ofx::dotfrag::Monochrome frag1;
	ofx::dotfrag::ThreeTones frag2;
	ofx::dotfrag::HSB frag3;

#ifdef INCLUDE_FX_DELAYS
	ofx::dotfrag::Delay frag4;
	ofx::dotfrag::EchoTrace frag5;
	//ofx::dotfrag::Twist frag6;
#endif

#endif

//	//-
//
//public:
//
	void setup();
	void update();
	void draw();
	void drawGui();
	void exit();
//
//	void keyPressed(int key);
//	void mouseMoved(int x, int y);
//	void mouseDragged(int x, int y, int button);
//	void mousePressed(int x, int y, int button);
//	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
//	void dragEvent(ofDragInfo dragInfo);
//	//void mouseEntered(int x, int y);
//	//void mouseExited(int x, int y);
//	//void gotMessage(ofMessage msg);
//
//private:
//	void startup();
//	void updateVideoPLayer();
//	//void updateScene();
//	//void drawVideoEffected();
//
//public:
//	void draw_Gui();
//	void draw_Video();
//	void draw_VideoControls();
//
//private:
//	//customize guiExtended
//	void customizeGuiSettingsDefine();
//	void customizeGuiSettingsApply();
//	bool bLoadedGuiFont = false;
//	ofJson jConf_Font;
//	ofJson jConf_Global;
//	ofJson jConf_BigBut1;//center text
//	ofJson jConf_BigBut2;//highlighted
//	ofJson jConf_BigBut3;//play
//	ofJson jConf_Labl;//labels
//	ofJson jConf_Highligthed;//highlighted color
//	ofJson jConf_Labl_Hide;//labels
//
//
//private:
//	//preset params
//	ofParameterGroup APP_params;
//
//	//-
//
//	//labels
//	//ofTrueTypeFont myFont;
//	string myTTF;// gui font for all gui theme
//	int sizeTTF;
//	string fname;
//
//	//--

	//gui
	ofxGui gui;
	//ofxGuiGroup2 *panel_ENGINE;

#ifdef INCLUDE_DOT_FRAG
	ofxGuiGroup2 *gui_VideoFx;
	ofxGuiGroup2 *gFrag1;
	ofxGuiGroup2 *gFrag2;
	ofxGuiGroup2 *gFrag3;
	ofxGuiGroup2 *gFrag4;
	ofxGuiGroup2 *gFrag5;
#endif
//
//	//video
//	ofParameter<string> videoFilePath;
//	ofParameter<string> videoName;
//
//	//--
//
//	//presetsManager
//	//start/finish video loops and more
//	void setup_PresetsManager();
//	ofxPresetsManager presetsManager;
//	ofParameterGroup params_ENGINE_Store;
//
//	//--
//
//	ofParameter<bool> SHOW_gui = true;//independent to autohide state
//	ofEventListener listener_SHOW_gui;
//	void Changed_SHOW_gui();
//
//	ofParameter<int> MODE_App;
//	ofEventListener listener_MODE_App;
//	void Changed_MODE_App();
//
	//settings
	//string path_fileName_AppSettings;
	void saveGroup(ofParameterGroup &g, string path);
	void loadGroup(ofParameterGroup &g, string path);
//
	ofParameter<int> window_W, window_H, window_X, window_Y;
};
