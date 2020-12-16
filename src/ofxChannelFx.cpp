#include "ofxChannelFx.h"

//--------------------------------------------------------------
void ofxChannelFx::fboAllocate()
{
	window_W = ofGetWidth();
	window_H = ofGetHeight();

	//-

	bool bArbPRE = ofGetUsingArbTex();
	ofDisableArbTex();
	{
		ofFbo::Settings fboSettings;
		fboSettings.width = window_W;
		fboSettings.height = window_H;
		fboSettings.internalformat = GL_RGBA;

		//TODO: check if we can add 3d compatibility..
		fboSettings.textureTarget = GL_TEXTURE_2D;

		fbo_FxChain.allocate(fboSettings);

		//clear
		fbo_FxChain.begin();
		ofClear(0, 0, 0, 255);
		fbo_FxChain.end();

		//-

		//fx
		frag1.allocate(fbo_FxChain);
		frag2.allocate(fbo_FxChain);
		frag3.allocate(fbo_FxChain);

#ifdef INCLUDE_FX_DELAYS
		frag4.allocate(fbo_FxChain);
		frag5.allocate(fbo_FxChain);
		//frag6.allocate(fbo_FxChain);
#endif
	}
	if (bArbPRE) ofEnableArbTex();
	else ofDisableArbTex();
}

//--------------------------------------------------------------
void ofxChannelFx::setup()
{
	DISABLE_Callbacks = true;

	fboAllocate();

	//-

	setup_FxChannel();

	//----

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


	//----

	//startup 
	startup();

}

//--------------------------------------------------------------
void ofxChannelFx::startup()
{
	ofLogNotice(__FUNCTION__);

	//-

	DISABLE_Callbacks = false;

	//-

	//load settings

	ofxSurfingHelpers::loadGroup(params_Session, path_GLOBAL_Folder + "/" + path_fileName_Session);

#ifndef INCLUDE_ofxPresetsManager
	ofxSurfingHelpers::loadGroup(params_Preset, path_GLOBAL_Folder + "/" + path_fileName_Preset);
#endif

	//-

#ifdef INCLUDE_ofxGuiExtended2
	guiPanel->setPosition(position_Gui.get().x, position_Gui.get().y);
	guiPanel->setShowHeader(bHeader.get());//required bc is not on xml settings
#endif

	//-

#ifdef USE_ofxPresetsManager
	presetsManager.refresh();
#endif

	//-

	//refresh

	bEnableGuiWorkflow = true;

	if (bEnableGuiWorkflow)
	{
		refresh_Gui();
	}
	else
	{
#ifdef INCLUDE_ofxGuiExtended2
		//minimize all
		gFrag1->minimize();
		gFrag2->minimize();
		gFrag3->minimize();

#ifdef INCLUDE_FX_DELAYS
		gFrag4->minimize();
		gFrag5->minimize();
		//gFrag6->minimize();
#endif

#endif
	}

#ifdef INCLUDE_ofxGui
	refresh_ofxGui_minimize();
#endif

	//setGuiPosition(glm::vec2(ofGetWidth() - 230, 10));
	setEnableKeys(true);
}

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

//--------------------------------------------------------------
void ofxChannelFx::setup_FxChannel()
{
	//parameters

	//customize names to tweak gui labels
	frag1.parameters.setName("1 " + frag1.parameters.getName());
	frag2.parameters.setName("2 " + frag2.parameters.getName());
	frag3.parameters.setName("3 " + frag3.parameters.getName());
#ifdef INCLUDE_FX_DELAYS
	frag4.parameters.setName("4 " + frag4.parameters.getName());
	frag5.parameters.setName("5 " + frag5.parameters.getName());
#endif

	//-

	//gui layout
	position_Gui.set("GUI POSITION",
		glm::vec2(window_W * 0.5, window_H * 0.5),
		glm::vec2(0, 0),
		glm::vec2(window_W, window_H)
	);

	//--

	//presets params

	params_Preset.setName("FX_CHANNEL");

	//fx
	//params_Preset.add(ENABLE_FxChain);

	params_Preset.add(frag1.parameters);
	params_Preset.add(frag2.parameters);
	params_Preset.add(frag3.parameters);

#ifdef INCLUDE_FX_DELAYS
	params_Preset.add(frag4.parameters);
	params_Preset.add(frag5.parameters);
	//params_Preset.add(frag6.parameters);
	SELECT_Fx.setMax(5);
#endif

	//exclude
	SELECT_Fx_Name.setSerializable(false);
	RESET.setSerializable(false);

	//--

	params_Session.setName("AppSession");
	params_Session.add(SELECT_Fx);
	params_Session.add(SELECT_Solo);
	params_Session.add(position_Gui);
	params_Session.add(bHeader);

	//gui

	//1. user

	params_Subcontrol.setName("CONTROL");
	params_Subcontrol.add(SELECT_Fx);
	params_Subcontrol.add(SELECT_Fx_Name);
	params_Subcontrol.add(SELECT_Solo);
	params_Subcontrol.add(RESET);

	//2. edit
	//linked enablers
	ENABLE_Monochrome.makeReferenceTo(frag1.active);
	ENABLE_ThreeTones.makeReferenceTo(frag2.active);
	ENABLE_HSB.makeReferenceTo(frag3.active);

	ENABLE_Monochrome.setName("ENABLE MONOCHROME");
	ENABLE_ThreeTones.setName("ENABLE THREETONES");
	ENABLE_HSB.setName("ENABLE HSB");
#ifdef INCLUDE_FX_DELAYS
	ENABLE_Delay.makeReferenceTo(frag4.active);
	ENABLE_Echotrace.makeReferenceTo(frag5.active);
	ENABLE_Delay.setName("ENABLE DELAY");
	ENABLE_Echotrace.setName("ENABLE ECHOTRACE");
#endif

	params_Subcontrol.add(ENABLE_Monochrome);
	params_Subcontrol.add(ENABLE_ThreeTones);
	params_Subcontrol.add(ENABLE_HSB);

	//extra fx
#ifdef INCLUDE_FX_DELAYS
	params_Subcontrol.add(ENABLE_Delay);
	params_Subcontrol.add(ENABLE_Echotrace);
#endif

	//-

	//main group for settings and callback
	params_Control.setName("ofxChannelFx");
	params_Control.add(params_Subcontrol);
	params_Control.add(params_Preset);

	//--

	//ofxGuiExtended
	//populate widgets
#ifdef INCLUDE_ofxGuiExtended2
	guiPanel = gui.addPanel("CHANNEL FX");
	guiGroup = guiPanel->addGroup("CHANNEL FX", ofJson{ {"show-header", false} });//avoid double header

	//main enabler
	guiGroup->add(ENABLE_FxChain);
	guiGroup->add(bMinimize);

#ifdef USE_ofxPresetsManager
	guiGroup->add(SHOW_Presets);
#endif

	guiGroup->add(ENABLE_Keys);
	guiGroup->add(bHeader);
#endif

	//-

	//main sub panels

#ifdef INCLUDE_ofxPresetsManager

#ifdef INCLUDE_ofxGuiExtended2
	gui_FxPresets = guiGroup->addGroup();
#endif

	params_PresetsManagerTools.add(presetsManager.getParamsControls());

#ifdef INCLUDE_ofxGuiExtended2
	gui_FxPresets->add(params_PresetsManagerTools);
	gui_FxPresets->minimize();
	gui_FxPresets->minimizeAll();
#endif

	//to use callbacks only
	params_Control.add(params_PresetsManagerTools);
#endif

	//--

#ifdef INCLUDE_ofxGuiExtended2
	//1. user
	gui_FxUser = guiGroup->addGroup(params_Subcontrol);

	//2. edit
	gui_FxEdit = guiGroup->addGroup("SETTINGS");
#endif

	//-

	//main
	//guiGroup->add(bHeader);//hidden from gui or not

	//to use callbacks only
	params_Control.add(bMinimize);
#ifdef USE_ofxPresetsManager
	params_Control.add(SHOW_Presets);
#endif
	params_Control.add(ENABLE_Keys);

#ifdef INCLUDE_ofxGuiExtended2
	params_Control.add(bHeader);//should be added to xml settings..
#endif

	//bHeader.setSerializable(true);
	bMinimize.setSerializable(false);

	//-

#ifdef INCLUDE_ofxGuiExtended2
	gFrag1 = gui_FxEdit->addGroup("FRAG1");
	gFrag2 = gui_FxEdit->addGroup("FRAG2");
	gFrag3 = gui_FxEdit->addGroup("FRAG3");

#ifdef INCLUDE_FX_DELAYS
	gFrag4 = gui_FxEdit->addGroup("FRAG4");
	gFrag5 = gui_FxEdit->addGroup("FRAG5");
#endif

	gFrag1->add(frag1.parameters);
	gFrag2->add(frag2.parameters);
	gFrag3->add(frag3.parameters);

#ifdef INCLUDE_FX_DELAYS
	gFrag4->add(frag4.parameters);
	gFrag5->add(frag5.parameters);
#endif

#endif

	//-

	parameters.setName("ofxChannelFx");
	parameters.add(params_Control);
	//parameters.add(params_ControlExternal);
	//parameters.add(params_Preset);

	//-

#ifdef INCLUDE_ofxGui
	ofxSurfingHelpers::setThemeDark_ofxGui();
	gui.setup();
	gui.add(parameters);
	gui.setPosition(10, 20);
#endif

	//-

	//customize extended gui
	setup_GuiTheme();

	//-

	//callback
	ofAddListener(params_Control.parameterChangedE(), this, &ofxChannelFx::Changed_params_Control);

	//-

#ifdef INCLUDE_ofxPresetsManager
	setup_PresetsManager();
#endif
}

#ifdef INCLUDE_ofxPresetsManager
//--------------------------------------------------------------
void ofxChannelFx::setup_PresetsManager()
{
	//customize
	presetsManager.setPath_UserKit_Folder(path_GLOBAL_Folder + "/ofxPresetsManager");
	//presetsManager.setPath_KitFolder("presets");
	//presetsManager.setPath_PresetsFolder("archive");
	//presetsManager.setPath_ControlSettings("settings");

	//user clicker
	presetsManager.setVisible_PresetClicker(true);
	presetsManager.setPosition_PresetClicker(200, 100, 50);

	//-

	//add group params
	presetsManager.add(params_Preset, { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' });
	presetsManager.setup("ofxChannelFx");//optional name

	//-

	params_ControlExternal.setName("CONTROLS");
	params_ControlExternal.add(ENABLE_FxChain);
	params_ControlExternal.add(presetsManager.getParamsControls());
}
#endif

//--------------------------------------------------------------
void ofxChannelFx::Changed_params_Control(ofAbstractParameter &e)
{
	if (!DISABLE_Callbacks)
	{
		std::string name = e.getName();
		//if (name != "" && name != "exclude")
		{
			ofLogNotice(__FUNCTION__) << name << " : " << e;
		}

		if (false) {}

		//----

		//select

		else if (name == "SELECT FX")
		{
			//if solo mutes all and enables selected
			if (SELECT_Solo.get())
			{
				frag1.active = false;
				frag2.active = false;
				frag3.active = false;
#ifdef INCLUDE_FX_DELAYS
				frag4.active = false;
				frag5.active = false;
#endif
				switch (SELECT_Fx.get())
				{
				case 1:
					frag1.active = true;
					break;

				case 2:
					frag2.active = true;
					break;

				case 3:
					frag3.active = true;
					break;
#ifdef INCLUDE_FX_DELAYS
				case 4:
					frag4.active = true;
					break;

				case 5:
					frag5.active = true;
					break;
#endif
				}
			}

			//set the name for selected
			switch (SELECT_Fx.get())
			{
			case 0:
				SELECT_Fx_Name = "";
				break;

			case 1:
				SELECT_Fx_Name = "1 MONOCHROME";
				break;

			case 2:
				SELECT_Fx_Name = "2 THREETONES";
				break;

			case 3:
				SELECT_Fx_Name = "3 HSB";
				break;

#ifdef INCLUDE_FX_DELAYS
			case 4:
				SELECT_Fx_Name = "4 DELAY";
				break;

			case 5:
				SELECT_Fx_Name = "5 ECHOTRACE";
				break;
#endif
			}

			//workflow
#ifdef INCLUDE_ofxGuiExtended2
			//maximize edit if minimized
			if (gui_FxEdit->getMinimized())
			{
				gui_FxEdit->maximize();
			}
#endif

			//--

			//TODO:
			//if (bEnableGuiWorkflow) 
			refresh_Gui();

			//--
		}

		//----

		//solo

		else if (name == "SOLO")
		{
			if (SELECT_Solo.get())
			{
				frag1.active = false;
				frag2.active = false;
				frag3.active = false;
#ifdef INCLUDE_FX_DELAYS
				frag4.active = false;
				frag5.active = false;
#endif
				switch (SELECT_Fx.get())
				{
				case 1:
					frag1.active = true;
					break;

				case 2:
					frag2.active = true;
					break;

				case 3:
					frag3.active = true;
					break;
#ifdef INCLUDE_FX_DELAYS
				case 4:
					frag4.active = true;
					break;

				case 5:
					frag5.active = true;
					break;
#endif
				}
			}
			else //TODO: to improve workflow we should restore previous (PRE) states...
			{
				switch (SELECT_Fx.get())
				{
				case 1:
				case 2:
				case 3:
#ifdef INCLUDE_FX_DELAYS
				case 4:
				case 5:
#endif
					frag1.active = true;
					frag2.active = true;
					frag3.active = true;
#ifdef INCLUDE_FX_DELAYS
					frag4.active = true;
					frag5.active = true;
#endif
					break;
				}
			}
		}

		//----

		//enabler toggles

		else if (name == ENABLE_Monochrome.getName())
		{
			if (ENABLE_Monochrome.get() && bEnableGuiWorkflow) {
				if (SELECT_Fx.get() != 1) {
					SELECT_Fx = 1;
				}
				refresh_ofxGuiExtended_Check();
			}
			else
			{
				refresh_Gui();
			}
		}
		else if (name == ENABLE_ThreeTones.getName() && bEnableGuiWorkflow)
		{
			if (ENABLE_ThreeTones.get()) {
				if (SELECT_Fx.get() != 2) {
					SELECT_Fx = 2;
				}
				refresh_ofxGuiExtended_Check();
			}
			else
			{
				refresh_Gui();
			}
		}
		else if (name == ENABLE_HSB.getName() && bEnableGuiWorkflow)
		{
			if (ENABLE_HSB.get()) {
				if (SELECT_Fx.get() != 3) {
					SELECT_Fx = 3;
				}
				refresh_ofxGuiExtended_Check();
			}
			else
			{
				refresh_Gui();
			}
		}
#ifdef INCLUDE_FX_DELAYS
		else if (name == ENABLE_Delay.getName() && bEnableGuiWorkflow)
		{
			if (ENABLE_Delay.get()) {
				if (SELECT_Fx.get() != 4) {
					SELECT_Fx = 4;
				}
				refresh_ofxGuiExtended_Check();
			}
			else
			{
				refresh_Gui();
			}
		}
		else if (name == ENABLE_Echotrace.getName() && bEnableGuiWorkflow)
		{
			if (ENABLE_Echotrace.get()) {
				if (SELECT_Fx.get() != 5) {
					SELECT_Fx = 5;
				}
				refresh_ofxGuiExtended_Check();
			}
			else
			{
				refresh_Gui();
			}
		}
#endif

		//----

		//reset

		else if (name == RESET.getName())
		{
			if (RESET.get()) {
				RESET = false;

				//reset all fx
				{
					frag1.active = false;
					frag1.low = 0;
					frag1.high = 1;

					frag2.active = false;
					frag2.mix = 1;
					frag2.thresholds[0] = 0;
					frag2.thresholds[1] = 0.5f;
					frag2.fade = 0.3f;
					frag2.colors[0] = 0;
					frag2.colors[1] = 128;
					frag2.colors[2] = 255;

					SELECT_Solo = 3;//select to maximize gui
					frag3.active = true;
					frag3.hue = 0;
					frag3.brightness = 0;
					frag3.contrast = 1;
					frag3.saturation = 1;

#ifdef INCLUDE_FX_DELAYS
					frag4.active = false;
					frag4.feedback = 0;
					frag4.blendmode = 0;

					frag5.active = false;
					frag5.gain = 0.7f;
					frag5.threshold = 0.55f;
					frag5.invert = false;
					frag5.hardCutoff = false;
#endif
				}

				//-

				//reset only the selected fx
//				switch (SELECT_Fx.get())
//				{
//				case 1:
//					frag1.active = true;
//					frag1.low = 0;
//					frag1.high = 1;
//					break;
//
//				case 2:
//					frag2.active = true;
//					frag2.mix = 1;
//					frag2.thresholds[0] = 0;
//					frag2.thresholds[1] = 0.5f;
//					frag2.fade = 0.3f;
//					frag2.colors[0] = 0;
//					frag2.colors[1] = 128;
//					frag2.colors[2] = 255;
//					break;
//
//				case 3:
//					frag3.active = true;
//					frag3.hue = 0;
//					frag3.brightness = 0;
//					frag3.contrast = 1;
//					frag3.saturation = 1;
//					break;
//
//#ifdef INCLUDE_FX_DELAYS
//				case 4:
//					frag4.active = true;
//					frag4.feedback = 0;
//					frag4.blendmode = 0;
//					break;
//
//				case 5:
//					frag5.active = true;
//					frag5.gain = 0.7f;
//					frag5.threshold = 0.55f;
//					frag5.invert = false;
//					frag5.hardCutoff = false;
//					break;
//#endif
//				}
			}
		}

		//----

		//minimize

		else if (name == bMinimize.getName())
		{
			if (bMinimize.get())
			{
				bMinimize = false;

#ifdef INCLUDE_ofxGuiExtended2
				gui_FxUser->minimize();
				gui_FxEdit->minimize();
#endif

#ifdef INCLUDE_ofxGui
				refresh_ofxGui_minimize();

				// minimize
				auto &g0 = gui.getGroup(params_Control.getName());
				auto &g1 = g0.getGroup(params_Control.getName());
				auto &gus = g1.getGroup(params_Session.getName());
				auto &gfx = g1.getGroup(params_Preset.getName());
				gus.minimize();
				gfx.minimize();
				gui.minimizeAll();
#endif
			}
		}

		//show presets

#ifdef USE_ofxPresetsManager
		else if (name == SHOW_Presets.getName())
		{
			presetsManager.setVisible_PresetClicker(SHOW_Presets);
		}
#endif

		//enable keys

		else if (name == ENABLE_Keys.getName())
		{
#ifdef USE_ofxPresetsManager
			presetsManager.setEnableKeys(ENABLE_Keys_Player);
#endif
		}

		//----

		//header

		else if (name == bHeader.getName())
		{
#ifdef INCLUDE_ofxGuiExtended2
			guiPanel->setShowHeader(bHeader.get());
#endif
		}
	}
}

//--------------------------------------------------------------
void ofxChannelFx::keyPressed(int key) {
	if (ENABLE_Keys) {

		if (key == OF_KEY_UP) {
			if (SELECT_Fx.get() > 0) SELECT_Fx--;
		}

		else if (key == OF_KEY_DOWN) {

#ifndef INCLUDE_FX_DELAYS
			if (SELECT_Fx.get() < 3) SELECT_Fx++;
#else
			if (SELECT_Fx.get() < 5) SELECT_Fx++;
#endif
		}

		else if (key == OF_KEY_BACKSPACE) {
			doReset();
		}
	}
}

//--------------------------------------------------------------
void ofxChannelFx::begin() {
	//if (ENABLE_FxChain)
	{
		fbo_FxChain.begin();
		ofClear(0, 0, 0, 0);
	}
}

//--------------------------------------------------------------
void ofxChannelFx::end() {
	//if (ENABLE_FxChain)
	{
		fbo_FxChain.end();
	}
}

//--------------------------------------------------------------
void ofxChannelFx::update_FxChannel()
{
	if (ENABLE_FxChain)
	{
		//fx
		frag1.apply(fbo_FxChain);
		frag3.apply(fbo_FxChain);
		frag2.apply(fbo_FxChain);

		//extra fx
#ifdef INCLUDE_FX_DELAYS
		frag4.apply(fbo_FxChain);
		frag5.apply(fbo_FxChain);
		//frag6.apply(fbo_FxChain);
#endif
	}
}

//--------------------------------------------------------------
void ofxChannelFx::windowResized(int _w, int _h)
{
	window_W = _w;
	window_H = _h;

	//resize fbo
	fboAllocate();
}

//--------------------------------------------------------------
void ofxChannelFx::drawGui() {

#ifdef INCLUDE_ofxGui
	if (SHOW_Gui.get()) {
		gui.draw();
	}
#endif

}

//--------------------------------------------------------------
void ofxChannelFx::draw()
{
	//if (ENABLE_FxChain)
	{
		update_FxChannel();

		ofSetColor(255, 255, 255, 255);

		if (!vflip) {
			fbo_FxChain.draw(0, 0, window_W, window_H);
		}
		else {
			fbo_FxChain.draw(0, window_H, window_W, -window_H);
		}
	}
}

//--------------------------------------------------------------
void ofxChannelFx::setup_GuiTheme()
{
#ifdef INCLUDE_ofxGuiExtended2

	//default
	guiPanel->setPosition(20, 20);

	//-

	//theme
	path_Theme = "assets/theme/";
	path_Theme += "theme_ofxGuiExtended2_01.json";
	loadTheme(path_Theme);

	//-

	//remove alpha slider from colors
	auto g0 = gFrag2->getGroup(frag2.parameters.getName())->getGroup("COLOR 0");
	auto g1 = gFrag2->getGroup(frag2.parameters.getName())->getGroup("COLOR 1");
	auto g2 = gFrag2->getGroup(frag2.parameters.getName())->getGroup("COLOR 2");

	g0 = gFrag2->getGroup("COLOR 0");
	g1 = gFrag2->getGroup("COLOR 1");
	g2 = gFrag2->getGroup("COLOR 2");
	auto a0 = g0->getControl("a");
	auto a1 = g1->getControl("a");
	auto a2 = g2->getControl("a");
	a0->setHidden(true);
	a1->setHidden(true);
	a2->setHidden(true);

	//a0->mini
	//g0->set();
	//g0->minimize();

	//-

	//customize
	j_ButtonBig =
	{
		//{"fill-color", "rgba(128,128,128,0.4)" },
		{"type", "fullsize"},
		{"height", 30},
		//{"text-align", "center"}
	};

	j_SliderBig =
	{
		//{"fill-color", "rgba(128,128,128,0.4)" },
		//{"type", "fullsize"},
		{"height", 35},
		//{"text-align", "center"}
	};

	(guiGroup->getControl(ENABLE_FxChain.getName()))->setConfig(j_ButtonBig);
	(guiGroup->getControl(ENABLE_FxChain.getName()))->setConfig({ {"text-align", "center"} });

	//1. user
	(gui_FxUser->getControl(SELECT_Fx.getName()))->setConfig(j_SliderBig);

	//2. editor
	(gui_FxUser->getControl(ENABLE_Monochrome.getName()))->setConfig(j_ButtonBig);
	(gui_FxUser->getControl(ENABLE_ThreeTones.getName()))->setConfig(j_ButtonBig);
	(gui_FxUser->getControl(ENABLE_HSB.getName()))->setConfig(j_ButtonBig);
#ifdef INCLUDE_FX_DELAYS
	(gui_FxUser->getControl(ENABLE_Delay.getName()))->setConfig(j_ButtonBig);
	(gui_FxUser->getControl(ENABLE_Echotrace.getName()))->setConfig(j_ButtonBig);
#endif

	gFrag1->getControl(ENABLE_Monochrome.getName())->setConfig(j_ButtonBig);
	gFrag2->getControl(ENABLE_ThreeTones.getName())->setConfig(j_ButtonBig);
	gFrag3->getControl(ENABLE_HSB.getName())->setConfig(j_ButtonBig);
#ifdef INCLUDE_FX_DELAYS
	gFrag4->getControl(ENABLE_Delay.getName())->setConfig(j_ButtonBig);
	gFrag5->getControl(ENABLE_Echotrace.getName())->setConfig(j_ButtonBig);
#endif

#endif
}

//--------------------------------------------------------------
void ofxChannelFx::refreshi_ofxGuiExtended_Minimize() {// for gui extended only!
#ifdef INCLUDE_ofxGuiExtended2
	gFrag1->minimize();
	gFrag2->minimize();
	gFrag3->minimize();
#ifdef INCLUDE_FX_DELAYS
	gFrag4->minimize();
	gFrag5->minimize();
#endif
#endif
}

//--------------------------------------------------------------
void ofxChannelFx::refresh_ofxGuiExtended_Check() {// for gui extended only!
	ofLogNotice(__FUNCTION__);

	//--

#ifdef INCLUDE_ofxGuiExtended2

	//all fx disabled
	if ((!ENABLE_Monochrome.get()) && (!ENABLE_HSB.get()) && (!ENABLE_ThreeTones.get())
#ifdef INCLUDE_FX_DELAYS
		&& (!ENABLE_Delay.get()) && (!ENABLE_Echotrace.get())
#endif
		)
	{
		gFrag1->minimize();
		gFrag2->minimize();
		gFrag3->minimize();
#ifdef INCLUDE_FX_DELAYS
		gFrag4->minimize();
		gFrag5->minimize();
#endif
	}
	//if some fx enabled and all are minimized
	else if (
		gFrag1->getMinimized() &&
		gFrag2->getMinimized() &&
		gFrag3->getMinimized()
#ifdef INCLUDE_FX_DELAYS
		&& gFrag4->getMinimized() &&
		gFrag5->getMinimized()
#endif
		)
	{
		switch (SELECT_Fx.get())
		{
		case 1:	if (ENABLE_Monochrome.get()) gFrag1->maximize(); break;
		case 2:	if (ENABLE_ThreeTones.get()) gFrag2->maximize(); break;
		case 3:	if (ENABLE_HSB.get()) gFrag3->maximize(); break;
#ifdef INCLUDE_FX_DELAYS
		case 4:	if (ENABLE_Delay.get()) gFrag4->maximize(); break;
		case 5:	if (ENABLE_Echotrace.get()) gFrag5->maximize(); break;
#endif
		}
	}
#endif
}
//--------------------------------------------------------------
void ofxChannelFx::refresh_ofxGui_minimize(bool bUseSolo)// for ofxGui mode only!
{
#ifdef INCLUDE_ofxGui
	auto &g0 = gui.getGroup(params_Control.getName());
	auto &g1 = g0.getGroup(params_Control.getName());
	auto &gus = g1.getGroup(params_Session.getName());
	auto &gfx = g1.getGroup(params_Preset.getName());

	//gus.minimize();
	//gfx.minimize();

	//frags
	auto &gd1 = gfx.getGroup(frag1.parameters.getName());
	auto &gd2 = gfx.getGroup(frag2.parameters.getName());
	auto &gd3 = gfx.getGroup(frag3.parameters.getName());
	gd1.minimize();
	gd2.minimize();
	gd3.minimize();

#ifdef INCLUDE_FX_DELAYS	
	auto &gd4 = gfx.getGroup(frag4.parameters.getName());
	auto &gd5 = gfx.getGroup(frag5.parameters.getName());
	//auto &gd6 = gfx.getGroup(frag6.parameters.getName());
	gd4.minimize();
	gd5.minimize();
	//gd6.minimize();
#endif

	if (bUseSolo) {
		//workflow
		//expand selected fx panel
		//if (SELECT_Solo.get())
		{
			switch (SELECT_Fx.get())
			{
			case 1:	if (ENABLE_Monochrome.get()) gd1.maximize(); break;
			case 2:	if (ENABLE_ThreeTones.get()) gd2.maximize(); break;
			case 3:	if (ENABLE_HSB.get()) gd3.maximize(); break;
#ifdef INCLUDE_FX_DELAYS
			case 4:	if (ENABLE_Delay.get()) gd4.maximize(); break;
			case 5:	if (ENABLE_Echotrace.get()) gd5.maximize(); break;
#endif
			}
		}
	}

#endif
}

//--------------------------------------------------------------
void ofxChannelFx::refresh_Gui()// extended or simple gui
{
	//TODO: maybe there's some refresh method duplicated..

	ofLogNotice(__FUNCTION__);

	//--

#ifdef INCLUDE_ofxGuiExtended2

	//minimize all
	gFrag1->minimize();
	gFrag2->minimize();
	gFrag3->minimize();
#ifdef INCLUDE_FX_DELAYS
	gFrag4->minimize();
	gFrag5->minimize();
	//gFrag6->minimize();
#endif

	//-

	//workflow
	//expand selected fx panel
	//if (SELECT_Solo.get())
	{
		switch (SELECT_Fx.get())
		{
		case 1:	if (ENABLE_Monochrome.get()) gFrag1->maximize(); break;
		case 2:	if (ENABLE_ThreeTones.get()) gFrag2->maximize(); break;
		case 3:	if (ENABLE_HSB.get()) gFrag3->maximize(); break;
#ifdef INCLUDE_FX_DELAYS
		case 4:	if (ENABLE_Delay.get()) gFrag4->maximize(); break;
		case 5:	if (ENABLE_Echotrace.get()) gFrag5->maximize(); break;
#endif
		}
	}
#endif

	//-

#ifdef INCLUDE_ofxGui
	refresh_ofxGui_minimize(true);
#endif
}

//--------------------------------------------------------------
void ofxChannelFx::exit()
{
#ifdef INCLUDE_ofxPresetsManager
	presetsManager.exit();
#endif

	//callback
	ofRemoveListener(params_Control.parameterChangedE(), this, &ofxChannelFx::Changed_params_Control);

	//settings 

#ifndef INCLUDE_ofxPresetsManager
	ofxSurfingHelpers::saveGroup(params_Preset, path_GLOBAL_Folder + "/" + path_fileName_Preset);
#endif

#ifdef INCLUDE_ofxGuiExtended2
	position_Gui = glm::vec2(guiPanel->getPosition().x, guiPanel->getPosition().y);
#endif

	ofxSurfingHelpers::saveGroup(params_Session, path_GLOBAL_Folder + "/" + path_fileName_Session);
}