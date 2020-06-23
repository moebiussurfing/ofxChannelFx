#include "ofxChannelFx.h"


//--------------------------------------------------------------
void ofxChannelFx::setup()
{
	//settings folder
	path_GLOBAL_Settings = "ofxChannelFx";
	//path_fileName_AppSettings = "AppSettings.xml";
	path_fileName_VideoFX = "videoFX.xml";

	//-

	window_W = ofGetWidth();
	window_H = ofGetHeight();
//	//modes. not included into presets
//
//#ifdef INCLUDE_DOT_FRAG
	ENABLE_DOT_FX.set("VIDEO FX ENABLE", true);
//	SHOW_DOT_FX.set("VIDEO FX EDIT", false);
//#endif
//
//#ifdef USE_GEN_PLAYER
//	SHOW_GEN.set("ENGINE MOOD EDIT", true);
//#endif
//

//
//	//modules
//
//	//-
//

//
//#ifdef INCLUDE_DOT_FRAG
//	params_ENGINE.add(ENABLE_DOT_FX);
//	params_ENGINE.add(SHOW_DOT_FX);
//#endif

//	//gui

	////ofxGuiExtended
	//customizeGuiSettingsDefine();
//

	//dot frag
#ifdef INCLUDE_DOT_FRAG
	setup_DotFrag();
#endif

//#ifdef INCLUDE_DOT_FRAG
//	APP_params.add(ENABLE_DOT_FX);
//	APP_params.add(SHOW_DOT_FX);
//	APP_params.add(SELECT_Fx);
//#endif
//
//#ifdef USE_GEN_PLAYER
//	APP_params.add(SHOW_GEN);
//#endif
//
//	//----
//
//	//callbacks
//
//	//gui
//	listener_SHOW_gui = SHOW_gui.newListener([this](bool &)
//	{
//		this->Changed_SHOW_gui();
//	});
//
//	//app mode manager
//	listener_MODE_App = MODE_App.newListener([this](int &)
//	{
//		this->Changed_MODE_App();
//	});
//
//	//--
//
//	//presetsManager
//
//	//for video mark/loops engine
//	setup_PresetsManager();
//
//	//---
//
//	//GUI
//	customizeGuiSettingsApply();
//
//	//----
//
//
//	//STARTUP 
//
//	startup();
//
//
//	//--
//
//	//HAP VIDEO PLAYER
//
//	//A. hardcoded video file
//	//videoFilePath="/Volumes/xTOSHIBA/VIDEO/NightmoVES4.mov";//default
//	//videoName="NightmoVES4";
//
//	////B. compose the path name from the name stored in the xml
//	////BUG: videoName do not loads because the string param is used too in the other group param above..
//	////is not stored, like if goes out of the param group
//	//auto myStrings = ofSplitString(videoFilePath, "/");//not working bc '\'
//	//videoName = myStrings[myStrings.size() - 1];
//
//	//split string path using boost
//	//https://stackoverflow.com/questions/10099206/howto-parse-a-path-to-a-vector-using-c
//	boost::filesystem::path p1(videoFilePath.get());
//	//boost::filesystem::path p1("/usr/local/bin");
//	////boost::filesystem::path p2("c:\\");
//	//std::cout << p1.filename() << std::endl; // prints "bin"
//	//std::cout << p1.parent_path() << std::endl; // prints "/usr/local"
//	videoName = ofToString(p1.filename());
//
//	//----
//
//	//load the path of the movie from xml settings
//	load(videoFilePath);
//
//	//hardcoded path file
//	//load("movies/NightmoVES4.mov");
//	//load("/Volumes/xTOSHIBA/VIDEO/NightmoVES4.mov");
//
//	player.setLoopState(OF_LOOP_NORMAL);
//	player.setVolume(0.0);
//
//	//----
//
//	//STARTUP 
//
//	//startup();
//	POSITION = start;
//
//	//----
}

////--------------------------------------------------------------
//void ofxChannelFx::startup()
//{
//	//startup init
//	presetsManager.refresh();
//
//	//load app settings
//	loadGroup(APP_params, path_GLOBAL_Settings + "/" + path_fileName_AppSettings);
//}
//
////--------------------------------------------------------------
//void ofxChannelFx::setActive(bool b)
//{
//	ENABLE_Active = b;
//	setGuiVisible(b);
//	setKeysEnable(b);
//}
//
////--------------------------------------------------------------
//void ofxChannelFx::setKeysEnable(bool b)
//{
//	ENABLE_Keys = b;
//
//	presetsManager.setEnableKeys(b);
//}

#ifdef INCLUDE_DOT_FRAG
//--------------------------------------------------------------
void ofxChannelFx::setup_DotFrag()
{
	////to avoid error on window resize that disables dotfrag..
	//fbo_VideoFx.allocate(window_W, window_H, GL_RGBA);

	//TODO:
	ofDisableArbTex();

	ofFbo::Settings fboSettings;
	fboSettings.width = window_W;
	fboSettings.height = window_H;
	fboSettings.internalformat = GL_RGBA;
	fboSettings.textureTarget = GL_TEXTURE_2D;
	fbo_VideoFx.allocate(fboSettings);

	//clear
	fbo_VideoFx.begin();
	ofClear(0, 0, 0, 255);
	fbo_VideoFx.end();

	//-

	//fx
	frag1.allocate(fbo_VideoFx);
	frag2.allocate(fbo_VideoFx);
	frag3.allocate(fbo_VideoFx);
	
#ifdef INCLUDE_FX_DELAYS
	frag4.allocate(fbo_VideoFx);
	frag5.allocate(fbo_VideoFx);
	//frag6.allocate(fbo_VideoFx);
#endif


	////TODO:
	//ofEnableArbTex();

	//--

	//params settings to store

	params_VideoFX.setName("VIDEO FX ENABLE");

	//fx
	params_VideoFX.add(frag1.parameters);
	params_VideoFX.add(frag2.parameters);
	params_VideoFX.add(frag3.parameters);
			
#ifdef INCLUDE_FX_DELAYS
	params_VideoFX.add(frag4.parameters);
	params_VideoFX.add(frag5.parameters);
	//params_VideoFX.add(frag6.parameters);
#endif

	//--

	//control params
	//disable for the momment
	//params_Control_VideoFX.setName("CONTROL");
	//params_Control_VideoFX.add(SELECT_Fx);

	//--

	//gui

	//main group

	ofParameterGroup params_GuiPanel;//to enable corrent folder header visible...
	params_GuiPanel.setName("DOT FX");

	//disable for the momment
	//params_GuiPanel.add(SELECT_Fx);

	//-

	////ofxGui
	////gui_VideoFx.setup("VIDEO FX ENABLE");
	////gui_VideoFx.add(params_VideoFX);

	//-

	//ofxGuiExtended

	gui_VideoFx = gui.addGroup(params_GuiPanel);

	gFrag1 = gui_VideoFx->addGroup("FRAG1");
	gFrag2 = gui_VideoFx->addGroup("FRAG2");
	gFrag3 = gui_VideoFx->addGroup("FRAG3");
			
#ifdef INCLUDE_FX_DELAYS
	gFrag4 = gui_VideoFx->addGroup("FRAG4");
	gFrag5 = gui_VideoFx->addGroup("FRAG5");
#endif

	gFrag1->add(frag1.parameters);
	gFrag2->add(frag2.parameters);
	gFrag3->add(frag3.parameters);
			
#ifdef INCLUDE_FX_DELAYS
	gFrag4->add(frag4.parameters);
	gFrag5->add(frag5.parameters);
#endif

	gui_VideoFx->setPosition(500, 20);

	//-

	////remove alpha slider from colors
	//auto gc1 = gFrag3->getGroup("COLOR 0");
	//auto a1 = gc1->getControl("a");
	//gc1->removeChild(a1);

	//auto gc2 = gFrag3->getGroup("COLOR 1");
	//auto a2 = gc2->getControl("r");
	//gc2->removeChild(a2);

	//auto gc3 = gFrag3->getGroup("COLOR 3");
	//auto a3 = gc3->getControl("r");
	//gc3->removeChild(a3);


	//auto gCol = gFrag3->getGroup("COLOR 0");
	//auto ga = gCol->getControl("a");
	//gFrag3->removeChild(ga);

	//-

	auto g0 = gFrag2->getGroup("COLOR 0");
	auto g1 = gFrag2->getGroup("COLOR 1");
	auto g2 = gFrag2->getGroup("COLOR 2");

	g0 = gFrag2->getGroup("COLOR 0");
	auto a0 = g0->getControl("a");
	a0->setEnabled(false);//only hides
	//g0->removeChild(a0);//crashes..
	//gFrag3->removeChild(a0);//do not deletes..

	g1 = gFrag2->getGroup("COLOR 1");
	auto a1 = g1->getControl("a");
	a1->setEnabled(false);
	//g1->removeChild(a0);

	g2 = gFrag2->getGroup("COLOR 2");
	auto a2 = g2->getControl("a");
	a2->setEnabled(false);
	//g2->removeChild(a2);

	//-

	//customize
	//gui_VideoFx->setConfig(jConf_Global);//not working

	//-

	//refresh
	refreshGuiDotFrag();

	//-

	//callback
	//disable for the momment
	//ofAddListener(params_Control_VideoFX.parameterChangedE(), this, &ofxChannelFx::Changed_params_VideoFX);

	//-

	//load
	loadGroup(params_VideoFX, path_GLOBAL_Settings + "/" + path_fileName_VideoFX);
}

//--------------------------------------------------------------
void ofxChannelFx::Changed_params_VideoFX(ofAbstractParameter &e) //patch change
{
	//string name = e.getName();
	//if (name != "" && name != "")
	//{ }
		//disable for the momment
		//if (name == "FX")
		//{
		//	refreshGuiDotFrag();
		//}
}
//--------------------------------------------------------------
void ofxChannelFx::begin() {
	if (ENABLE_DOT_FX)
	{
		ofDisableArbTex();
		fbo_VideoFx.begin();
		ofClear(0, 0, 0, 0);
	}
}

//--------------------------------------------------------------
void ofxChannelFx::end() {
	if (ENABLE_DOT_FX)
	{
		//-

		fbo_VideoFx.end();

		//-

		//fx
		frag1.apply(fbo_VideoFx);
		frag3.apply(fbo_VideoFx);
		frag2.apply(fbo_VideoFx);
		
#ifdef INCLUDE_FX_DELAYS
		frag4.apply(fbo_VideoFx);
		frag5.apply(fbo_VideoFx);
		//frag6.apply(fbo_VideoFx);
#endif
	}
}

//--------------------------------------------------------------
void ofxChannelFx::update_DotFrag()
{
	if (ENABLE_DOT_FX)
	{
		ofDisableArbTex();
		fbo_VideoFx.begin();
		ofClear(0, 0, 0, 0);

		//-

		////original raw
		//draw_Video();

		//-

		fbo_VideoFx.end();

		//-

		//fx
		frag1.apply(fbo_VideoFx);
		frag2.apply(fbo_VideoFx);
		frag3.apply(fbo_VideoFx);
		
#ifdef INCLUDE_FX_DELAYS
		frag4.apply(fbo_VideoFx);
		frag5.apply(fbo_VideoFx);
		//frag6.apply(fbo_VideoFx);
#endif
	}
}
#endif
//
////--------------------------------------------------------------
//void ofxChannelFx::update()
//{
//	//--
//
//	//gui
//	updateVideoPLayer();
//
//	//--
//
//#ifdef USE_GEN_PLAYER
//	GEN.update();
//#endif
//
//	//--
//}
//
////--------------------------------------------------------------
//void ofxChannelFx::calculateKick()
//{
//	//kick-drift
//	//calculate desired step size
//	//by left-right arrow keys
//
//	//just try, bc if video is not loaded will need to do it again
//	totalNumFrames = player.getTotalNumFrames();
//	if (totalNumFrames == 0)
//		ofLogError("ofxChannelFx") << "totalNumFrames is 0. Could wait until video file is loaded...";
//	else
//		ofLogNotice("ofxChannelFx") << "totalNumFrames: " << totalNumFrames;
//
//	frameSizeNorm = 1.0f / (float)totalNumFrames;//decimals size of a frame
//	ofLogNotice("ofxChannelFx") << "frameSizeNorm: " << frameSizeNorm;
//	kickSizeFrame = frameSizeNorm * (float)numFramesToKick;
//	ofLogNotice("ofxChannelFx") << "kickSizeFrame: " << kickSizeFrame;
//
//	//float currDur = player.getDuration();//step proportional to video secs
//	//currFrame = ofMap(POSITION, 0, 1, 0, totalNumFrames);//curr frame?
//}
//
////--------------------------------------------------------------
//void ofxChannelFx::updateVideoPLayer()
//{
//	POSITION = player.getPosition();
//
//	//-
//
//	if (totalNumFrames == 0)
//	{
//		calculateKick();
//	}
//
//	//-
//
//	//display current time in minute:seconds. max minutes is 99 to nice formatting
//	positionSeconds = player.getDuration() * POSITION;
//
//	//time
//	int currMin, currSec;
//	string strMin, strSec;
//	currMin = positionSeconds / 60;
//	currSec = ((int)(positionSeconds)) % 60;
//	strMin = (currMin < 10) ? "0" : "";
//	strSec = (currSec < 10) ? "0" : "";
//	strMin += ofToString(currMin);
//	strSec += ofToString(currSec);
//	videoTIME = "time " + strMin + ":" + strSec;//string
//
//	//frame
//	videoTIME += " | frame " + ofToString(POSITION*totalNumFrames, 0);
//
//	//--
//
//	//check if player is running
//	bool isPlaying = player.isPlaying() && !player.isPaused();
//
//	//TODO:
//	//BUG: last/first frame get flicked..
//
//	if (isPlaying)//playing. player running
//	{
//		//cout << "isPlaying: " << isPlaying << endl;
//
//		//loop engine
//		//TODO: 
//		//workflow: added !MODE_EDIT to allow playing out of range loop
//		//but requires to imrpove workflow when playing preset with EDIT MODE enabled
//		if (ENABLE_LOOP)// && !MODE_EDIT)
//		{
//			//if (player.getPosition() >= finish-0.001f)
//			if (player.getPosition() >= finish)
//			{
//				if (!looped)
//				{
//					if (reverseSpeed)
//						POSITION = finish;
//					else
//						POSITION = start;
//				}
//				else
//				{
//					POSITION = finish;
//					reverseSpeed = !reverseSpeed;
//				}
//
//				player.setPosition(POSITION);
//			}
//			else if (player.getPosition() <= start)//? < only
//			{
//				if (!looped)
//				{
//					if (reverseSpeed)
//						POSITION = finish;
//					else
//						POSITION = start;
//				}
//				else
//				{
//					POSITION = start;
//					reverseSpeed = !reverseSpeed;
//				}
//
//				player.setPosition(POSITION);
//			}
//		}
//	}
//
//	else//not playing. player stopped
//	{
//		if (!inScrub && !ENABLE_skiper && !player.isPaused())
//		{
//			//clamp cursor into loop
//			if (ENABLE_LOOP && !MODE_EDIT)
//			{
//				if (player.getPosition() >= finish)
//				{
//					POSITION = finish;
//					player.setPosition(POSITION);
//				}
//				else if (player.getPosition() <= start)
//				{
//					POSITION = start;
//					player.setPosition(POSITION);
//				}
//			}
//		}
//	}
//
//	//--
//
//	////auto hide gui system
//
//	//if (ofGetSystemTimeMillis() - lastMovement < time_autoHide)
//	//{
//	//	draw_Autohide_PRE = ENABLE_drawVideoControlBar;
//	//	ENABLE_drawVideoControlBar = true;
//	//}
//	//else
//	//{
//	//	if (ENABLE_AutoHide)
//	//	{
//	//		ENABLE_drawVideoControlBar = false;
//	//		draw_Autohide_PRE = ENABLE_drawVideoControlBar;
//	//	}
//	//}
//
//	//--
//
//	////TODO
//	////disable to avoid bug with clicks l/r on gui (ofxGuiExtended2)
//	////hide mouse if changed
//	//if ((ENABLE_drawVideoControlBar != draw_Autohide_PRE) || ENABLE_AutoHide)
//	//{
//	//    ofRectangle window = ofGetWindowRect();
//	//    if (!ENABLE_drawVideoControlBar && window.inside(ofGetMouseX(), ofGetMouseY()))
//	//    {
//	//        ofHideCursor();
//	//    }
//	//    else
//	//    {
//	//        ofShowCursor();
//	//    }
//	//}
//
//	//--
//
//	//SKIPPER ENGINE:
//
//	//time
//	if (ENABLE_skiper && MODE_SkipTime && !inScrub && PLAYING)
//	{
//		if (ofGetSystemTimeMillis() - last_TRIG_time >= timePeriod_time)
//		{
//			last_TRIG_time = ofGetSystemTimeMillis();
//			//trig jump skip
//			TRIG_time_Skiper = true;
//
//			//workflow
//			if (MODE_SkipReverse)
//			{
//				last_TRIG_reverse = ofGetSystemTimeMillis();
//				TRIG_reverse_Skiper = true;
//			}
//		}
//	}
//
//	//reverse
//	if (ENABLE_skiper && MODE_SkipReverse && !inScrub && PLAYING)
//	{
//		if (ofGetSystemTimeMillis() - last_TRIG_reverse >= timePeriod_reverse)
//		{
//			last_TRIG_reverse = ofGetSystemTimeMillis();
//			//trig jump skip
//			TRIG_reverse_Skiper = true;
//		}
//	}
//
//	//--
//
//	//time trigger
//	if (TRIG_time_Skiper == true)
//	{
//		TRIG_time_Skiper = false;
//
//		//if (MODE_SkipTime)
//		{
//			float skipPos;
//			if (ENABLE_LOOP)
//			{
//				skipPos = ofRandom(start, finish);
//			}
//			else
//			{
//				skipPos = ofRandom(0.0, 1.0);
//			}
//			//cout << "skipPos: " << skipPos << endl;
//			player.setPosition(skipPos);
//		}
//	}
//
//	//reverse trigger
//	if (TRIG_reverse_Skiper == true)
//	{
//		TRIG_reverse_Skiper = false;
//
//		//if (MODE_SkipReverse)
//		{
//			reverseSpeed = !reverseSpeed;
//		}
//	}
//
//	//--
//}
//
////--------------------------------------------------------------
//void ofxChannelFx::draw_Gui()
//{
//#ifdef USE_GEN_PLAYER
//	//if (SHOW_GEN)
//	{
//		GEN.drawPreview();
//	}
//#endif
//}
//
////--------------------------------------------------------------
//void ofxChannelFx::setGuiVisible(bool b)
//{
//	//TODO:
//	//ENABLE_GuiVisibleByAutoHide = b;
//	//if (b)
//	//{
//	//	draw_Autohide_PRE = ENABLE_drawVideoControlBar;
//	//	ENABLE_drawVideoControlBar = true;
//	//	lastMovement = ofGetSystemTimeMillis();
//	//}
//
//	//draw control bar
//	ENABLE_drawVideoControlBar = b;
//
//	//-
//
//	//presets
//	presetsManager.setVisible_PresetClicker(b);
//	presetsManager.setVisible_GUI_Browser(b);
//	presetsManager.setEnableKeys(b);
//	if (!b)//only if must hide
//	{
//		presetsManager.setVisible_GUI_Internal(b);
//	}
//
//	//-
//
//	//engine
//	gui.getVisible().set(b);//all
//	panel_ENGINE->getVisible().set(b);
//	if (b && SHOW_DOT_FX)
//		gui_VideoFx->getVisible().set(true);
//	else
//		gui_VideoFx->getVisible().set(false);
//
//#ifdef USE_GEN_PLAYER
//	GEN.setGui_visible(b && SHOW_GEN);
//#endif
//}
//
//
//#pragma mark - KEY & MOUSE CALLBACKS
//
////--------------------------------------------------------------
//void ofxChannelFx::mouseMoved(int x, int y)
//{
//	if (ENABLE_Active)
//	{
//		{
//			lastMovement = ofGetSystemTimeMillis();
//		}
//	}
//}
//
////--------------------------------------------------------------
//void ofxChannelFx::mouseDragged(int x, int y, int button)
//{
//	if (ENABLE_Active)
//	{
//		lastMovement = ofGetSystemTimeMillis();
//
//		if (inScrub)
//		{
//			float position = static_cast<float>(x - BarInset) / getBarRectangle().width;
//			position = std::max(0.0f, std::min(position, 1.0f));
//			player.setPosition(position);
//
//			POSITION = position;
//
//			//workflow:
//			//disable loop if cursor is out-of-loop
//			if (POSITION < start || POSITION > finish)
//			{
//				if (ENABLE_LOOP && !MODE_EDIT) ENABLE_LOOP = false;
//			}
//		}
//	}
//}
//
////--------------------------------------------------------------
//void ofxChannelFx::mousePressed(int x, int y, int button)
//{
//	if (ENABLE_Active)
//	{
//		ofRectangle bar = getBarRectangle();
//		if (bar.inside(x, y))
//		{
//			ofLogVerbose("ofxChannelFx") << "mousePressed INSIDE: (" << x << "," << y << ")";
//
//			inScrub = true;
//			wasPaused = player.isPaused() || player.getIsMovieDone();
//
//			//workflow
//			//auto-stop
//			if (PLAYING)
//			{
//				PLAYING = false;
//			}
//			mouseDragged(x, y, button);
//		}
//		else
//		{
//			//ofLogNotice("ofxChannelFx") << "OUTSIDE";
//		}
//
//		lastMovement = ofGetSystemTimeMillis();
//	}
//}
//
////--------------------------------------------------------------
//void ofxChannelFx::mouseReleased(int x, int y, int button)
//{
//	if (ENABLE_Active)
//	{
//		if (inScrub)
//		{
//			inScrub = false;
//
//			//stop
//			//player.setPaused(true);
//			//PLAYING = false;
//			//player.setPaused(wasPaused);
//		}
//	}
//}
//
//--------------------------------------------------------------
void ofxChannelFx::windowResized(int _w, int _h)
{
	window_W = _w;
	window_H = _h;
//
//	//-
//
//	int xx, yy, xPad, yPad;
//	xx = 2 * BarInset;//default hardcoded pos
//	yy = 10;
//	yPad = 5;
//	xPad = 5;
//
//	//-
//
//	//gui layout
//
//	//left aligned
//	panel_ENGINE->setPosition(xx, yy);//anchor panel
//
//	//related from bottom
//	//float ww = panel_ENGINE->getHeight();
//	//panel_ENGINE->setPosition(xx, _h - ww - 200);//left aligned
//
//	//-
//
//	//presetsManager
//	//clicker centered
//	float wp = window_W * 0.5f - presetsManager.getPresetClicker_Width()*0.5f;
//	presetsManager.setPosition_PresetClicker(wp, window_H - 250, 50);
//	//presetsManager.set_CLICKER_Position(800, window_H - 150, 50);
//
//	//--
//
//	//populate all gui panels from left to right of previous one
//
//#ifdef USE_GEN_PLAYER
//	auto gPos = panel_ENGINE->getPosition();
//	auto gWidth = panel_ENGINE->getWidth();
//	GEN.setPosition(gPos.x + gWidth + xPad, gPos.y);//right to the user panel
//
//	float pw = 400;
//	float px = window_W * 0.5f - pw * 0.5f;//center on screen
//	GEN.setPreviewPosition(px, 20, pw, 40);//custom preview position
//#endif
//
//	//-
//
////#ifdef USE_GEN_PLAYER
////	auto gGenWidth = GEN.getGuiUserWidth();//right to the GEN panel
////	gui_VideoFx->setPosition((gPos.x + gWidth) + gGenWidth + xPad + xPad, gPos.y);
////#else
////	float gGenWidth = 200;
////	float gWidth = 200;
////	glm::vec2 gPos = glm::vec2(300+ gGenWidth);
////	gui_VideoFx->setPosition((gPos.x + gWidth) + gGenWidth + xPad + xPad, gPos.y);
////#endif
//
//	//TODO: crossed ifdef's..
//#ifdef INCLUDE_DOT_FRAG
//
//#ifdef USE_GEN_PLAYER
//	auto gGenWidth = GEN.getGuiUserWidth();//right to the GEN panel
//	gui_VideoFx->setPosition((gPos.x + gWidth) + gGenWidth + xPad + xPad, gPos.y);
//#else
//	auto www = panel_ENGINE->getWidth();
//	auto xxx = panel_ENGINE->getPosition().x;
//	gui_VideoFx->setPosition(xxx + www + 5, 10);
//#endif
//
//	//--
//
//	//TODO:
//	ofDisableArbTex();
//	ofFbo::Settings fboSettings;
//	fboSettings.width = window_W;
//	fboSettings.height = window_H;
//	fboSettings.internalformat = GL_RGBA;
//	fboSettings.textureTarget = GL_TEXTURE_2D;
//	fbo_VideoFx.allocate(fboSettings);
//
//	//clear
//	fbo_VideoFx.begin();
//	ofClear(0, 0, 0, 255);
//	fbo_VideoFx.end();
//
//	//restore arb mode state
//	ofEnableArbTex();
//
//	//--
//
//#endif
//
}
//
////--------------------------------------------------------------
//void ofxChannelFx::keyPressed(int key)
//{
//	if (ENABLE_Keys)
//	{
//		//----
//
//		//main panels show/hide
//
//		if (key == OF_KEY_F1)
//		{
//			bool b = !panel_ENGINE->getVisible().get();
//			panel_ENGINE->getVisible().set(b);
//			//if (b)
//			ENABLE_drawVideoControlBar = b;
//		}
//#ifdef USE_GEN_PLAYER
//		else if (key == OF_KEY_F2)
//		{
//			SHOW_GEN = !SHOW_GEN;
//		}
//#endif
//		else if (key == OF_KEY_F3)
//		{
//			SHOW_DOT_FX = !SHOW_DOT_FX;
//		}
//		else if (key == OF_KEY_F4)
//		{
//			presetsManager.setVisible_PresetClicker(!presetsManager.isVisible_PresetClicker());
//		}
//#ifdef USE_GEN_PLAYER
//		else if (key == OF_KEY_F5)
//		{
//			GEN.togglePreviewVisible();
//		}
//#endif
//		else if (key == OF_KEY_F6)
//		{
//			ENABLE_drawVideoControlBar = !ENABLE_drawVideoControlBar;
//		}
//
//		//----
//
//		//TODO:
//		//must add modifiers to avoid keys collapsing...
//		//bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
//
//		//if (ENABLE_Active)
//		//if (MODE_App == 0)
//
//		//-
//
//		//transport
//
//		//play-stop
//		else if (key == ' ')
//		{
//			setPlay(!PLAYING);
//		}
//
//		else if (key == OF_KEY_RETURN)
//		{
//#ifdef USE_GEN_PLAYER
//			bool b = !GEN.isPlaying();
//			setPlay_GEN(b);
//			setPlay(b);
//#endif
//		}
//
//		//-
//
//		//loop mode
//		else if (key == 'l' || key == 'L')
//		{
//			ENABLE_LOOP = !ENABLE_LOOP;
//		}
//
//		//edit mode
//		else if (key == 'E' || key == 'e')
//		{
//			MODE_EDIT = !MODE_EDIT;
//
//			//ENABLE_LOOP = !ENABLE_LOOP;
//			////if (!ENABLE_LOOP && ENABLE_skiper)
//			////{
//			////	ENABLE_skiper = false;
//			////}
//		}
//
//		//workflow
//		//auto-enable edit mode if it's disabled
//		if ((key == 'i' || key == 'o') && !MODE_EDIT)
//		{
//			MODE_EDIT = true;
//		}
//
//		//edit mode
//		if (MODE_EDIT)
//		{
//			if (false) {}
//
//			//store (set)
//			else if (key == 'i')
//			{
//				MARK_START = true;
//			}
//			else if (key == 'o')
//			{
//				MARK_FINISH = true;
//			}
//
//			//recall (go)
//			else if (key == 'I')
//			{
//				POSITION = start;
//			}
//			else if (key == 'O')
//			{
//				POSITION = finish;
//			}
//
//			//user kick-drift frame-by-frame
//			else if (key == OF_KEY_LEFT && MODE_EDIT)
//			{
//				POSITION -= kickSizeFrame;
//			}
//			else if (key == OF_KEY_RIGHT && MODE_EDIT)
//			{
//				POSITION += kickSizeFrame;
//			}
//
//			//reset preset. basic settings only
//			else if (key == 'r' || key == 'R')
//			{
//				TRIG_Mods_Reset = true;
//			}
//		}
//
//		//-
//
//		//helpers triggers
//		else if (key == 's')
//		{
//			TRIG_time_Skiper = true;
//		}
//		else if (key == 'S')
//		{
//			ENABLE_skiper = !ENABLE_skiper;
//		}
//
//		//autohide
//		//else if (key == 'a' && !ENABLE_AutoHide_external)
//		//{
//		//	ENABLE_AutoHide = !ENABLE_AutoHide;
//		//	if (ENABLE_AutoHide)
//		//	{
//		//		//ENABLE_drawVideoControlBar=true;
//		//		lastMovement = ofGetSystemTimeMillis();
//		//	}
//		//}
//
//		//-
//
//		//-
//
//		//sound
//		else if (key == OF_KEY_UP)
//		{
//			player.setVolume(player.getVolume() + 0.1);
//		}
//		else if (key == OF_KEY_DOWN)
//		{
//			player.setVolume(player.getVolume() - 0.1);
//		}
//
//		//-
//
//		//show/hide gui
//		else if (key == 'g')
//		{
//			SHOW_gui = !SHOW_gui;//independent to autohide state
//
//	//		//hide/show cursor
//	//		if (!SHOW_gui)
//	//		{
//	//			ofHideCursor();
//	//		}
//	//		else
//	//		{
//	//			ofShowCursor();
//	//		}
//	//
//	//#ifdef USE_GEN_PLAYER
//	//		/*GEN.setGui_visible(ENABLE_drawVideoControlBar && SHOW_GEN);*/
//	//		GEN.setGui_visible(SHOW_GEN);
//	//#endif
//
//		}
//	}
//}
//
////--------------------------------------------------------------
//void ofxChannelFx::dragEvent(ofDragInfo dragInfo)//drag video to load another one
//{
//	vector<string> fileList = dragInfo.files;
//	videoName = ofToString(ofFilePath::getFileName(fileList[0]));
//	videoFilePath = ofFilePath::getAbsolutePath(fileList[0]);
//
//	ofLogNotice("ofxChannelFx") << "video title: " << videoName;
//	ofLogNotice("ofxChannelFx") << "video file path: " << videoFilePath;
//	load(fileList[0]);
//}
//
//#pragma mark - PRESETS MANAGER
//
////--------------------------------------------------------------
//void ofxChannelFx::setup_PresetsManager()
//{
//	presetsManager.setPath_GlobalFolder("ofxChannelFx/ofxPresetsManager");
//	presetsManager.setPath_KitFolder("presets");
//	presetsManager.setPath_ControlSettings("settings");
//	presetsManager.setPath_PresetsFolder("archive");
//
//	presetsManager.add(params_ENGINE_Store,
//		//{ '0', '1', '2', '3', '4', '5', '6', '7' });
//		{ 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',' });
//
//	//to customize name gui panel
//	presetsManager.setup("ofxChannelFx");
//	//group name goes to prefix name file presets
//
//	//easy callback when save/load is done
//	presetsManager.DONE_save.addListener(this, &ofxChannelFx::Changed_DONE_save);
//	presetsManager.DONE_load.addListener(this, &ofxChannelFx::Changed_DONE_load);
//
//	//layout
//	presetsManager.setVisible_PresetClicker(true);//default
//	presetsManager.setVisible_GUI_Internal(false);//default
//}
//
////--------------------------------------------------------------
//void ofxChannelFx::Changed_DONE_save(bool &DONE_save)
//{
//	ofLogNotice("ofxChannelFx") << "Changed_DONE_save: " << ofToString(DONE_save ? "TRUE" : "FALSE");
//}
//
////--------------------------------------------------------------
//void ofxChannelFx::Changed_DONE_load(bool &DONE_load)
//{
//	ofLogNotice("ofxChannelFx") << "Changed_DONE_load: " << ofToString(DONE_load ? "TRUE" : "FALSE");
//
//	if (presetsManager.isDoneLoad())
//	{
//		POSITION = start;
//		player.setPosition(POSITION);
//
//		//-
//
//		if (!ENABLE_LOOP)
//			ENABLE_LOOP = true;
//	}
//}
//
//#pragma mark - CHANGED
//
////--------------------------------------------------------------
//void ofxChannelFx::Changed_params_ENGINE(ofAbstractParameter &e) //patch change
//{
//	string name = e.getName();
//	if (name != "" &&
//		name != "TRIG TIME SKIP" &&
//		name != "TRIG REVERSE SKIP" &&
//		name != "POSITION" &&
//		name != "REVERSE" &&
//		name != "")
//		ofLogNotice("ofxChannelFx") << "Changed_params_ENGINE '" << name << "': " << e;
//
//	if (name == "PLAY")
//	{
//		if (!PLAYING)
//		{
//			player.setPaused(true);
//		}
//		else
//		{
//			player.setPaused(false);
//
//			//workflow
//			if (!ENABLE_LOOP)
//			{
//				ENABLE_LOOP = true;
//			}
//			if (MODE_EDIT)
//			{
//				MODE_EDIT = false;
//			}
//		}
//	}
//	if (name == "EDIT")
//	{
//		if (MODE_EDIT)
//		{
//			//workflow
//			if (!ENABLE_LOOP) ENABLE_LOOP = true;
//			presetsManager.setEnableKeysArrowBrowse(false);
//		}
//		else
//		{
//			presetsManager.setEnableKeysArrowBrowse(true);
//		}
//	}
//	else if (name == "LOOP")
//	{
//		if (ENABLE_LOOP)
//		{
//			player.setPosition(start);
//		}
//	}
//	else if (name == "MARK START" && MARK_START)
//	{
//		MARK_START = false;
//		start = player.getPosition();
//
//		if (MODE_EDIT)
//		{
//			//workflow
//			//enable loop
//			if (!ENABLE_LOOP)
//				ENABLE_LOOP = true;
//
//			//workflow
//			//if star/end flipped: set finish one second to the right
//			if (finish < start)
//			{
//				float gap = frameSizeNorm * 60;//1sec at 60fps
//				finish = start + gap;
//			}
//		}
//	}
//	else if (name == "MARK FINISH" && MARK_FINISH)
//	{
//		MARK_FINISH = false;
//		finish = player.getPosition();
//
//		//workflow
//		//if star/end flipped: set finish one second to the right
//		if (finish < start)
//		{
//			float gap = frameSizeNorm * 60;//1sec at 60fps
//			start = finish - gap;
//		}
//	}
//
//	//-
//
//	else if (name == "POSITION")
//	{
//		player.setPosition(POSITION);
//	}
//
//	//TODO
//	//BUG: breaks preset save/load...
//	//else if (name == "START")
//	//{
//	//    if (start > finish)
//	//    {
//	//        float TEMP = start;
//	//        start = finish;
//	//        finish = start;
//	//    }
//	//}
//	//else if (name == "FINISH")
//	//{
//	//    if (finish < start)
//	//    {
//	//        float TEMP = finish;
//	//        finish = start;
//	//        start = finish;
//	//    }
//	//}
//
//	else if (name == "SPEED")
//	{
//		player.setSpeed((reverseSpeed ? (-1.0f) : (1.0f)) * speed);
//	}
//	else if (name == "RESET SPEED")
//	{
//		speed_Reset = false;
//		speed = 1.0f;
//	}
//	else if (name == "RESET MODS")
//	{
//		TRIG_Mods_Reset = false;
//
//		looped = false;
//		reverseSpeed = false;
//		ENABLE_skiper = false;
//		MODE_SkipTime = false;
//		MODE_SkipReverse = false;
//		timePeriod_time = 1000;
//		timePeriod_reverse = 1000;
//		speed = 1.0f;
//	}
//	else if (name == "REVERSE")
//	{
//		player.setSpeed((reverseSpeed ? (-1.0f) : (1.0f)) * speed);
//
//		//if (reverseSpeed)
//		//{
//		//}
//	}
//
//#ifdef USE_GEN_PLAYER
//	else if (name == "ENGINE MOOD EDIT")
//	{
//		GEN.setGui_visible(SHOW_GEN);
//}
//#endif
//
//#ifdef INCLUDE_DOT_FRAG
//	else if (name == "VIDEO FX ENABLE")
//	{
//		////workflow
//		//if (!ENABLE_DOT_FX)
//		//	SHOW_DOT_FX = false;
//	}
//	else if (name == "VIDEO FX EDIT")
//	{
//		bool b = SHOW_DOT_FX.get();
//		gui_VideoFx->setVisible(b);
//
//		//ofxGui
//		//if (ENABLE_DOT_FX && SHOW_gui && ENABLE_GuiVisibleByAutoHide)
//		//{
//		//	  gui_VideoFx.draw();
//		//}
//	}
//
//#endif
//}
//
////--------------------------------------------------------------
//void ofxChannelFx::Changed_SHOW_gui()
//{
//	ofLogNotice("ofxChannelFx") << "Changed_SHOW_gui:" << ofToString(SHOW_gui.get());
//
//	//hide all
//	if (!SHOW_gui)
//	{
//		setGuiVisible(false);
//	}
//	//show all player
//	else
//	{
//		//draw_Autohide_PRE = ENABLE_drawVideoControlBar;
//		//ENABLE_drawVideoControlBar = true;
//		//lastMovement = ofGetSystemTimeMillis();
//
//		//-
//
//		setGuiVisible(true);
//	}
//}
//
////--------------------------------------------------------------
//void ofxChannelFx::Changed_MODE_App()
//{
//	//ofLogNotice("ofxChannelFx") << "Changed_MODE_App:" << ofToString(MODE_App.get());
//
//	//lastMovement = ofGetSystemTimeMillis();
//
//	//if (MODE_App == 1)
//	//{
//	//	//hide all player
//	//	setGuiVisible(false);
//	//}
//	//else if (MODE_App == 0)
//	//{
//	//	//show all player
//	//	if (SHOW_gui)
//	//	{
//	//		setGuiVisible(true);
//	//	}
//	//}
//}
////
//////--------------------------------------------------------------
////void ofxChannelFx::Changed_draw_Autohide(bool &b)
////{
////	//	if (ENABLE_AutoHide)
////	//	{
////	//		if (ENABLE_drawVideoControlBar != draw_Autohide_PRE)
////	//		{
////	//			ofLogNotice("ofxChannelFx") << "Changed_draw_Autohide: " << ofToString(ENABLE_drawVideoControlBar ? "TRUE" : "FALSE");
////	//
////	//			if (MODE_App == 0)
////	//			{
////	//				if (!SHOW_gui && ENABLE_drawVideoControlBar)
////	//				{
////	//					draw_Autohide_PRE = ENABLE_drawVideoControlBar;
////	//					ENABLE_drawVideoControlBar = false;
////	//				}
////	//				setGuiVisible(ENABLE_drawVideoControlBar);
////	//
////	//#ifdef USE_GEN_PLAYER
////	//				//GEN.setGui_visible(ENABLE_drawVideoControlBar&&SHOW_GEN);
////	//#endif
////	//			}
////	//			else if (MODE_App == 1)
////	//			{
////	//				if (!SHOW_gui && ENABLE_drawVideoControlBar)
////	//				{
////	//					draw_Autohide_PRE = ENABLE_drawVideoControlBar;
////	//					ENABLE_drawVideoControlBar = false;
////	//				}
////	//			}
////	//		}
////	//
////	//		//hide/show cursor
////	//		if (!ENABLE_drawVideoControlBar)
////	//		{
////	//			ofHideCursor();
////	//		}
////	//		else
////	//		{
////	//			ofShowCursor();
////	//		}
////	//	}
////}
//
//#pragma mark MOOD_GEN
//
//#ifdef USE_GEN_PLAYER
////GEN
////listeners for inside class GEN
////--------------------------------------------------------------
//void ofxChannelFx::Changed_GEN_TARGET(int &targetVal)
//{
//	ofLogNotice("ofxChannelFx") << "Changed_GEN_TARGET: " << targetVal;
//}
//
////--------------------------------------------------------------
//void ofxChannelFx::Changed_GEN_PRESET_A(int &targetVal)
//{
//	ofLogNotice("ofxChannelFx") << "Changed_GEN_PRESET_A: " << targetVal;
//
//	//-
//
//	//TODO
//	////1. power off all glitchers:
//	//GLITCH_power(0, false);
//	//
//	////2. power on selected glitcher
//	//GLITCH_power(targetVal + 1, true);
//
//	//-
//}
//
////-------------------------------------------------
//void ofxChannelFx::Changed_GEN_PRESET_C(int &targetVal)
//{
//	ofLogNotice("ofxChannelFx") << "Changed_GEN_PRESET_C: " << targetVal;
//
//	presetsManager.PRESET_selected = targetVal + 1;
//}
//
////-------------------------------------------------
////callback to receive mood states
////void ofxChannelFx::Changed_GEN_MOOD_RANGE(int & targetVal)
////{
////    ofLogNotice("ofxChannelFx") << "> Changed_GEN_MOOD_RANGE: " << targetVal;
////
////    if (targetVal == 0)
////    {
////        ofBackground(color_MOOD1);
////    }
////    if (targetVal == 1)
////    {
////        ofBackground(color_MOOD2);
////    }
////    if (targetVal == 2)
////    {
////        ofBackground(color_MOOD3);
////    }
////
////}
//#endif
//
//#pragma mark - VIDEO
//
//

//--------------------------------------------------------------
void ofxChannelFx::drawGui(){

}


//--------------------------------------------------------------
void ofxChannelFx::draw()
{
//#ifdef INCLUDE_DOT_FRAG
	//ofxDotFrag
	if (ENABLE_DOT_FX)
	{
		//update_DotFrag();

		ofSetColor(255, 255, 255, 255);
		fbo_VideoFx.draw(0, 0, window_W, window_H);
	}

//	//raw clean
//	else
//#endif
//	{
//		//ofEnableArbTex();
//		//draw_Video();
//	}
}

////--------------------------------------------------------------
//void ofxChannelFx::draw_Video()
//{
//	if (player.isLoaded())
//	{
//		ofPushStyle();
//
//		//draw video frame
//		ofSetColor(255, 255, 255, 255);
//
//		ofRectangle videoRect(0, 0, player.getWidth(), player.getHeight());
//		videoRect.scaleTo(ofGetWindowRect());//fill window width
//
//		//--
//
//		player.draw(videoRect.x, videoRect.y, videoRect.width, videoRect.height);
//
//		ofPopStyle();
//	}
//}
//
//
////--------------------------------------------------------------
//void ofxChannelFx::draw_VideoControls()
//{
//	if (player.isLoaded())
//	{
//		int alphaBar = 128;
//		float roundB = 5.0f;
//
//		//draw the position bar if appropriate
//		if (ENABLE_drawVideoControlBar)
//		{
//			ofPushStyle();
//
//			//-
//
//			//1. border rect only. full video timeline
//			ofNoFill();
//			ofRectangle barFull = getBarRectangle();
//			ofSetColor(ofColor::white, alphaBar);
//			//ofDrawRectangle(barFull);
//			ofDrawRectRounded(barFull, roundB);
//
//			//-
//
//			//2. filled rectangle from time 0 to current time position
//			ofFill();
//			ofSetColor(ofColor::white, alphaBar);
//			ofRectangle barCurTime = getBarRectangle();;
//			barCurTime.width = barFull.width * player.getPosition();
//			//ofDrawRectangle(barCurTime);
//			ofDrawRectRounded(barCurTime, roundB);
//
//			int yy = ofGetWindowHeight() - BarInset - BarHeight;
//
//			//don't draw loop bar if loop not enable
//			if (ENABLE_LOOP)
//			{
//				//3. markers loop rectangle: from start to end
//				ofSetColor(ofColor(64), 192);//darker grey
//				int pStart, pWidth;
//				int padding = 2;
//				pStart = BarInset + barFull.width * start;
//				pWidth = (BarInset + barFull.width * finish) - pStart;
//				ofRectangle barLoop = ofRectangle(pStart, yy + padding, pWidth, BarHeight - padding * 2);
//				ofDrawRectangle(barLoop);
//
//				//3.2 red line markes to loop
//				if (MODE_EDIT)
//				{
//					ofNoFill();
//					ofSetLineWidth(2.0);
//					ofSetColor(ofColor::red);
//					//int padding = 1;
//					ofDrawLine(pStart, yy + padding, pStart, yy + BarHeight - 1);
//					ofDrawLine(pStart + pWidth, yy + padding, pStart + pWidth, yy + BarHeight - 1);
//				}
//			}
//
//			//-
//
//			//4. red line for current video player time
//			ofNoFill();
//			ofSetLineWidth(3.0);
//			ofSetColor(ofColor::red);
//			float posTime = barCurTime.width + BarInset;
//			int padding = 3;
//			ofDrawLine(posTime, yy - padding, posTime, yy + BarHeight + padding);
//
//			//-
//
//			ofPopStyle();
//		}
//	}
//	else
//	{
//		if (player.getError().length())
//		{
//			ofDrawBitmapStringHighlight(player.getError(), 20, 20);
//		}
//		else
//		{
//			ofDrawBitmapStringHighlight("MOVIE IS LOADING...", 20, 20);
//			cout << "ERROR ";
//		}
//	}
//}
//
////--------------------------------------------------------------
//void ofxChannelFx::load(std::string movie)
//{
//	bool b = player.load(movie);
//
//	if (b)
//	{
//		ofLogNotice("ofxChannelFx") << "LOADED VIDEO '" << movie << "'";
//	}
//	else
//	{
//		ofLogError("ofxChannelFx") << "VIDEO NOT FOUND '" << movie << "' !";
//	}
//
//	calculateKick();//just try bc if video is not loaded will need to do again
//
//	lastMovement = ofGetSystemTimeMillis();
//}
//
////--------------------------------------------------------------
//ofRectangle ofxChannelFx::getBarRectangle() const
//{
//	return ofRectangle(
//		BarInset, ofGetWindowHeight() - BarInset - BarHeight,
//		ofGetWindowWidth() - (2 * BarInset), BarHeight);
//}
//
//
#pragma mark - SETTINGS

//--------------------------------------------------------------
void ofxChannelFx::loadGroup(ofParameterGroup &g, string path)
{
	ofLogNotice("ofxChannelFx") << "loadGroup " << path;
	ofXml settings;
	settings.load(path);
	ofDeserialize(settings, g);
}

//--------------------------------------------------------------
void ofxChannelFx::saveGroup(ofParameterGroup &g, string path)
{
	ofLogNotice("ofxChannelFx") << "saveGroup " << path;
	ofXml settings;
	ofSerialize(settings, g);
	settings.save(path);
}

////gui customize
////--------------------------------------------------------------
//void ofxChannelFx::customizeGuiSettingsDefine()
//{
//	//--
//
//	//ofxGui
//
//	//fname = "telegrama_render.otf";
//	fname = "overpass-mono-bold.otf";
//
//	string path = path_GLOBAL_Settings + "/" + "fonts/" + fname;
//	ofFile file(path);
//	if (file.exists())
//	{
//		ofxGuiSetFont(path, 8);
//	}
//	else
//	{
//		ofLogError("ofxChannelFx") << "ofxGui theme FONT FILE '" << path << "' NOT FOUND!";
//	}
//	ofxGuiSetDefaultHeight(18);
//	ofxGuiSetBorderColor(16);
//	ofxGuiSetFillColor(ofColor(48));
//	ofxGuiSetTextColor(ofColor::white);
//	ofxGuiSetHeaderColor(ofColor(32));
//
//	//--
//
//	//ofxGuiExtended
//	string myTTF;
//	int sizeTTF = 8;//font size
//
//	fname = "overpass-mono-bold.otf";
//	//fname = "PragmataProR_0822.ttf";
//	//fname = "telegrama_render.otf";
//
//	myTTF = path_GLOBAL_Settings + "/" + "fonts/" + fname;
//
//	ofFile fileF(myTTF);
//	bLoadedGuiFont = fileF.exists();
//	if (bLoadedGuiFont)
//	{
//		jConf_Font =
//		{
//			{"font-size", sizeTTF},
//			{"font-family", myTTF},
//			//{"text-padding", 4},//expands widget too..
//			//{"text-padding", 8},//expands widget too..
//			//{"text-align", "center"},
//			//{"border-radius", 2},
//			{"precision", 3}
//		};
//	}
//	else
//	{
//		ofLogError("ofxChannelFx") << "ofxGuiExtended theme FONT FILE '" << myTTF << "' NOT FOUND!";
//	}
//
//	//--
//
//	//widgets styles
//	//int gui_button_h = 22;
//
//	jConf_Global =
//	{
//		{"background-color", "rgba(255,0,0,0.5)"},
//		{"height", 10}
//		//{"margin", 3},
//		//{"text-padding", 3},
//		//{"text-align", "center"},
//	};
//
//	jConf_BigBut1 =
//	{
//		{"fill-color", "rgba(128,128,128,0.4)" },
//		{"type", "fullsize"},
//		{"text-align", "center"},
//		{"height", 22},
//	};
//
//	jConf_BigBut2 =//highlighted darken widgets: fx enablers, position,
//	{
//		{"fill-color", "rgba(128,128,128,0.4)" },
//		//{"fill-color", "rgba(64,64,64,0.5)"},
//		//{"border-color", "rgba(128,128,128,0.5)"},
//		{"type", "fullsize"},
//		//{"text-align", "left"},
//		{"text-align", "right"},
//		//{"border-width", 2.0f},
//		//{"border-radius", 2},
//		{"height", 35},
//	};
//
//	jConf_BigBut3 =//play button
//	{
//		{"fill-color", "rgba(128,128,128,0.4)" },
//		//{"fill-color", "rgba(64,64,64,0.5)"},
//		//{"border-color", "rgba(128,128,128,0.5)"},
//		{"type", "fullsize"},
//		{"text-align", "center"},
//		//{"border-width", 2.0f},
//		//{"border-radius", 2},
//		{"height", 45},
//	};
//
//	jConf_Highligthed =//highlighted yellow color
//	{
//		//{"text-color", "rgba(200,200,0,0.7)" }
//		//{"background-color", "rgba(200,200,0,0.2)" }
//		{"border-color", "rgba(200,200,0,0.4)" }
//	};
//
//	jConf_Labl =//labels highlighted (yellow)
//	{
//		//{"type", "fullsize"},
//		{"text-align", "center"},
//		{"height", 20},
//	};
//
//	jConf_Labl_Hide =//hiden labels
//	{
//		{"show-name", false},
//		{"text-align", "center"},
//		{"height", 5},
//	};
//}
//
////--------------------------------------------------------------
//void ofxChannelFx::customizeGuiSettingsApply()
//{
//	//apply first to all widgets
//	//some widgets will overwrite after?
//	//panel_ENGINE->setConfig(jConf_Global);//not working bc applies to container!
//
//	//-
//
//	(panel_ENGINE->getToggle("VIDEO FX ENABLE"))->setConfig(jConf_BigBut2);
//	(panel_ENGINE->getToggle("VIDEO FX EDIT"))->setConfig(jConf_BigBut2);
//
//	(panel_ENGINE->getToggle("PLAY"))->setConfig(jConf_BigBut3);
//	(panel_ENGINE->getToggle("EDIT"))->setConfig(jConf_BigBut3);
//	(panel_ENGINE->getToggle("LOOP"))->setConfig(jConf_BigBut1);
//	(panel_ENGINE->getFloatSlider("POSITION"))->setConfig(jConf_BigBut1);
//	(panel_ENGINE->getFloatSlider("POSITION"))->unregisterMouseEvents();
//
//	(panel_ENGINE->getToggle("MARK START"))->setConfig(jConf_BigBut1);
//	(panel_ENGINE->getToggle("MARK FINISH"))->setConfig(jConf_BigBut1);
//
//	(panel_ENGINE->getToggle("LOOPED BACK"))->setConfig(jConf_BigBut1);
//	(panel_ENGINE->getToggle("REVERSE"))->setConfig(jConf_BigBut1);
//
//	(panel_ENGINE->getToggle("ENABLE SKIPPERS"))->setConfig(jConf_BigBut1);
//	(panel_ENGINE->getToggle("A MODE TIME"))->setConfig(jConf_BigBut1);
//
//	(panel_ENGINE->getToggle("TRIG TIME SKIP"))->setConfig(jConf_BigBut1);
//	(panel_ENGINE->getToggle("B MODE REVERSE"))->setConfig(jConf_BigBut1);
//	(panel_ENGINE->getToggle("TRIG REVERSE SKIP"))->setConfig(jConf_BigBut1);
//
//	//highlighted
//	(panel_ENGINE->getToggle("ENABLE SKIPPERS"))->setConfig(jConf_Highligthed);
//	(panel_ENGINE->getToggle("A MODE TIME"))->setConfig(jConf_Highligthed);
//	(panel_ENGINE->getToggle("B MODE REVERSE"))->setConfig(jConf_Highligthed);
//
//	(panel_ENGINE->getIntSlider("SKIP-TIME TIME"))->setConfig(jConf_BigBut1);
//	(panel_ENGINE->getIntSlider("SKIP-REVERSE TIME"))->setConfig(jConf_BigBut1);
//
//	(panel_ENGINE->getFloatSlider("START"))->setConfig(jConf_BigBut1);
//	(panel_ENGINE->getFloatSlider("FINISH"))->setConfig(jConf_BigBut1);
//	(panel_ENGINE->getFloatSlider("SPEED"))->setConfig(jConf_BigBut1);
//
//	(panel_ENGINE->getControl("ENGINE"))->setConfig(jConf_Labl);
//	(panel_ENGINE->getControl("SKIP TIMERS"))->setConfig(jConf_Labl);
//	(panel_ENGINE->getControl("FX"))->setConfig(jConf_Labl);
//
//	(panel_ENGINE->getControl("_spacer1_"))->setConfig(jConf_Labl_Hide);
//	(panel_ENGINE->getControl("_spacer2_"))->setConfig(jConf_Labl_Hide);
//
//#ifdef USE_GEN_PLAYER
//	(panel_ENGINE->getToggle("ENGINE MOOD EDIT"))->setConfig(jConf_BigBut2);
//#endif
//
//	//-
//
//	//disable for the momment
//	//(gui_VideoFx->getIntSlider("FX"))->setConfig({
//	//	//{"padding", 10},
//	//	//{"margin", "10 20 30 40"},//widget allocation
//	//	//{"text-align", "right"},//not works here
//	//	//{"text-padding", 20},
//	//	//{"border-radius", 3},//rounded
//	//	//{"height", 100},
//	//	{"update-on-release-only", true}//works slow..
//	//	});
//
//	//-
//
//	////custom gui font to all inside all gui panels
//	//if (bLoadedGuiFont)
//	//{
//	//	//gui.setConfig(jConf_Font);
//	//}
//
//	//-
//
//	////containers
//	//panel_ENGINE->setWidth(200);
//	//gui_VideoFx->setWidth(200);
//
//	//-
//
//	//TODO:
//	//gui theme
//	{
//		ofLogNotice("ofxChannelFx") << "loadTheme";
//		panel_ENGINE->loadTheme("theme/theme_bleurgh.json");
//		gui_VideoFx->loadTheme("theme/theme_bleurgh.json");
//	}
//}

//--------------------------------------------------------------
void ofxChannelFx::refreshGuiDotFrag()
{
	ofLogNotice("ofxChannelFx") << "refreshGuiDotFrag()";

	//collapse all

	/*
	//ofxGui
	//auto &g = gui_VideoFx.getGroup("VIDEO FX ENABLE");
	//g.getGroup("monochrome").minimize();
	//g.getGroup("HSB").minimize();
	//g.getGroup("three tones").minimize();
	////fx
	////g.getGroup("video delay").minimize();
	////g.getGroup("echotrace").minimize();										 "a"
	////g.getGroup("twist").minimize();

	//switch (SELECT_Fx)
	//{
	//case 0:
	//	g.getGroup("monochrome").maximize();
	//	g.getGroup("HSB").maximize();
	//	g.getGroup("three tones").maximize();
	//	break;
	//case 1:
	//	g.getGroup("monochrome").maximize();
	//	break;
	//case 2:
	//	g.getGroup("HSB").maximize();
	//	break;
	//case 3:
	//	g.getGroup("three tones").maximize();
	//	break;
	//}
	*/

	//-

	//ofxGuiExtended
	gFrag1->minimize();
	gFrag2->minimize();
	gFrag3->minimize();
			
#ifdef INCLUDE_FX_DELAYS
	gFrag4->minimize();
	gFrag5->minimize();
#endif

	//disable for the momment
	////collapse all
	//gFrag1->minimize();
	//gFrag2->minimize();
	//gFrag3->minimize();

	////expand selected fx panel
	//switch (SELECT_Fx)
	//{
	//case 0:
	//	gFrag1->maximize();
	//	gFrag2->maximize();
	//	gFrag3->maximize();
	//	break;

	//case 1:	gFrag1->maximize(); break;
	//case 2:	gFrag2->maximize(); break;
	//case 3:	gFrag3->maximize(); break;
	//}
}

//--------------------------------------------------------------
void ofxChannelFx::exit()
{
	////settings

	////save app settings
	//saveGroup(APP_params, path_GLOBAL_Settings + "/" + path_fileName_AppSettings);

	//video fx
#ifdef INCLUDE_DOT_FRAG
	saveGroup(params_VideoFX, path_GLOBAL_Settings + "/" + path_fileName_VideoFX);
#endif

//	//-
//
//	//callbacks
//
//	ofRemoveListener(params_ENGINE.parameterChangedE(), this, &ofxChannelFx::Changed_params_ENGINE);
//	//disable for the momment
//	//ofRemoveListener(params_Control_VideoFX.parameterChangedE(), this, &ofxChannelFx::Changed_params_VideoFX);
//
//	////auto hide callback
//	//ENABLE_drawVideoControlBar.removeListener(this, &ofxChannelFx::Changed_draw_Autohide);
//
//	//-
//
//	//presetsManager
//	presetsManager.DONE_save.removeListener(this, &ofxChannelFx::Changed_DONE_save);
//	presetsManager.DONE_load.removeListener(this, &ofxChannelFx::Changed_DONE_load);
//	presetsManager.exit();
//
//	//-
//
//#ifdef USE_GEN_PLAYER
//	GEN.TARGET_Selected.removeListener(this, &ofxChannelFx::Changed_GEN_TARGET);//listener for inside class GEN
//	GEN.PRESET_A_Selected.removeListener(this, &ofxChannelFx::Changed_GEN_PRESET_A);//listener for inside class GEN
//	GEN.PRESET_C_Selected.removeListener(this, &ofxChannelFx::Changed_GEN_PRESET_C);//listener for inside class GEN
//	GEN.exit();
//#endif

}