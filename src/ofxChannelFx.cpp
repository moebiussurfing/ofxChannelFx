#include "ofxChannelFx.h"

//--------------------------------------------------------------
void ofxChannelFx::setup()
{

	DISABLE_Callbacks = true;


	//-

	window_W = ofGetWidth();
	window_H = ofGetHeight();

	//-

	bArbPRE = ofGetUsingArbTex();
	ofDisableArbTex();
	{
		ofFbo::Settings fboSettings;
		fboSettings.width = window_W;
		fboSettings.height = window_H;
		fboSettings.internalformat = GL_RGBA;
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

	//load settings

	loadGroup(params_Session, path_GLOBAL_Folder + "/" + path_fileName_Session);

#ifndef INCLUDE_ofxPresetsManager
	loadGroup(params_Preset, path_GLOBAL_Folder + "/" + path_fileName_Preset);
#endif

	guiPanel->setShowHeader(bHeader.get());//required bc is not on xml settings

	//-

#ifdef USE_ofxPresetsManager
	presetsManager.refresh();
#endif

	//-

	//refresh
	refreshGui_FxChannel();
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

	//--

	//presets params

	params_Preset.setName("FX CHAIN");
	ENABLE_FxChain.set("ENABLE", true);

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

	//gui

	//1. user
	params_Session.setName("> USER");

	params_Session.add(SELECT_Fx);
	params_Session.add(SELECT_Fx_Name);
	params_Session.add(SELECT_Solo);
	params_Session.add(RESET);

	//2. edit
	//linked enablers
	ENABLE_Monochrome.makeReferenceTo(frag1.active);
	ENABLE_ThreeTones.makeReferenceTo(frag2.active);
	ENABLE_HSB.makeReferenceTo(frag3.active);

	ENABLE_Monochrome.setName("ENABLE MONOCHROME");
	ENABLE_ThreeTones.setName("ENABLE THREETONES");
	ENABLE_HSB.setName("ENABLE HSB");

	params_Session.add(ENABLE_Monochrome);
	params_Session.add(ENABLE_ThreeTones);
	params_Session.add(ENABLE_HSB);

	//extra fx
#ifdef INCLUDE_FX_DELAYS
	ENABLE_Delay.makeReferenceTo(frag4.active);
	ENABLE_Echotrace.makeReferenceTo(frag5.active);

	ENABLE_Delay.setName("ENABLE DELAY");
	ENABLE_Echotrace.setName("ENABLE ECHOTRACE");

	params_Session.add(ENABLE_Delay);
	params_Session.add(ENABLE_Echotrace);
#endif

	//-

	//main group for settings and callback
	params_Control.setName("ofxChannelFx");
	params_Control.add(params_Session);
	params_Control.add(params_Preset);

	//--

	//ofxGuiExtended
	//populate widgets

	guiPanel = gui.addPanel("ofxChannelFx");
	guiGroup = guiPanel->addGroup("ofxChannelFx");

	//main enabler
	guiGroup->add(ENABLE_FxChain);
	guiGroup->add(bMinimize);

	//-

	//main sub panels

#ifdef INCLUDE_ofxPresetsManager
	gui_FxPresets = guiGroup->addGroup();
	params_PresetsManagerTools.add(presetsManager.PRESET_selected);
	params_PresetsManagerTools.add(presetsManager.SHOW_ClickPanel);
	params_PresetsManagerTools.add(presetsManager.bRandomizeEditor);
	//params_PresetsManagerTools.add(presetsManager.bRandomizeSelect);
	//params_Session.add(params_PresetsManagerTools);

	gui_FxPresets->add(params_PresetsManagerTools);
	gui_FxPresets->minimize();
	gui_FxPresets->minimizeAll();

	//to use callbacks only
	params_Control.add(params_PresetsManagerTools);
#endif

	//1. user
	gui_FxUser = guiGroup->addGroup(params_Session);

	//2. edit
	gui_FxEdit = guiGroup->addGroup("> EDIT");

	//-

	//main
	//guiGroup->add(bHeader);//hidden from gui or not

	//to use callbacks only
	//bMinimize.set("MINIMIZE");
	params_Control.add(bMinimize);
	params_Control.add(bHeader);//should be added to xml settings..

	//bHeader.setSerializable(true);
	bMinimize.setSerializable(false);

	//-

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

	//-

	//customize
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
	presetsManager.setPath_GlobalFolder(path_GLOBAL_Folder + "/ofxPresetsManager");
	//presetsManager.setPath_KitFolder("presets");
	//presetsManager.setPath_PresetsFolder("archive");
	//presetsManager.setPath_ControlSettings("settings");

	//user clicker
	presetsManager.setVisible_PresetClicker(true);
	presetsManager.setPosition_PresetClicker(200, 100, 50);

	//-

	//add group params
	presetsManager.add(params_Preset, { '0', '1', '2', '3', '4', '5', '6', '7', '8' });
	presetsManager.setup("ofxChannelFx");//optional name
}
#endif

//--------------------------------------------------------------
void ofxChannelFx::Changed_params_Control(ofAbstractParameter &e)
{
	if (!DISABLE_Callbacks)
	{
		string name = e.getName();
		if (name != "" && name != "exclude")
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
				SELECT_Fx_Name = "MONOCHROME";
				break;

			case 2:
				SELECT_Fx_Name = "THREETONES";
				break;

			case 3:
				SELECT_Fx_Name = "HSB";
				break;

#ifdef INCLUDE_FX_DELAYS
			case 4:
				SELECT_Fx_Name = "DELAY";
				break;

			case 5:
				SELECT_Fx_Name = "ECHOTRACE";
				break;
#endif
			}

			//workflow
			//maximize edit if minimized
			if (gui_FxEdit->getMinimized())
			{
				gui_FxEdit->maximize();
			}

			//--

			//TODO:
			refreshGui_FxChannel();

			//--
		}

		//----

		//solo

		else if (name == "SOLO")
		{
			if (SELECT_Solo.get()) {
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

		else if (name == "ENABLE MONOCHROME")
		{
			if (ENABLE_Monochrome.get()) {
				if (SELECT_Fx.get() != 1) {
					SELECT_Fx = 1;
				}
			}
			refreshGuiCollapse_FxChannel();
		}
		else if (name == "ENABLE THREETONES")
		{
			if (ENABLE_ThreeTones.get()) {
				if (SELECT_Fx.get() != 2) {
					SELECT_Fx = 2;
				}
			}
			refreshGuiCollapse_FxChannel();
		}
		else if (name == "ENABLE HSB")
		{
			if (ENABLE_HSB.get()) {
				if (SELECT_Fx.get() != 3) {
					SELECT_Fx = 3;
				}
			}
			refreshGuiCollapse_FxChannel();
		}
#ifdef INCLUDE_FX_DELAYS
		else if (name == "ENABLE DELAY")
		{
			if (ENABLE_Delay.get()) {
				if (SELECT_Fx.get() != 4) {
					SELECT_Fx = 4;
				}
			}
			refreshGuiCollapse_FxChannel();
		}
		else if (name == "ENABLE ECHOTRACE")
		{
			if (ENABLE_Echotrace.get()) {
				if (SELECT_Fx.get() != 5) {
					SELECT_Fx = 5;
				}
			}
			refreshGuiCollapse_FxChannel();
		}
#endif

		//----

		//reset

		else if (name == "RESET")
		{
			if (RESET.get()) {
				RESET = false;

				switch (SELECT_Fx.get())
				{
				case 1:
					frag1.active = true;
					frag1.low = 0;
					frag1.high = 1;
					break;

				case 2:
					frag2.active = true;
					frag2.mix = 1;
					frag2.thresholds[0] = 0;
					frag2.thresholds[1] = 0.5f;
					frag2.fade = 0.3f;
					frag2.colors[0] = 0;
					frag2.colors[1] = 128;
					frag2.colors[2] = 255;
					break;

				case 3:
					frag3.active = true;
					frag3.hue = 0;
					frag3.brightness = 0;
					frag3.contrast = 1;
					frag3.saturation = 1;
					break;

#ifdef INCLUDE_FX_DELAYS
				case 4:
					frag4.active = true;
					frag4.feedback = 0;
					frag4.blendmode = 0;
					break;

				case 5:
					frag5.active = true;
					frag5.gain = 0.7f;
					frag5.threshold = 0.55f;
					frag5.invert = false;
					frag5.hardCutoff = false;
					break;
#endif
				}
			}
		}

		//----

		//minimize

		else if (name == "MINIMIZE")
		{
			if (bMinimize.get())
			{
				bMinimize = false;

				if (gui_FxPresets->getMinimized() && gui_FxUser->getMinimized() && gui_FxEdit->getMinimized())
				{
					gui_FxUser->maximize();
					//return;
				}
				else
				{
					gui_FxPresets->minimize();
					gui_FxUser->minimize();
					gui_FxEdit->minimize();
				}
			}
		}

		//----

		//header

		else if (name == "HEADER")
		{
			guiPanel->setShowHeader(bHeader.get());
		}
	}
}

//--------------------------------------------------------------
void ofxChannelFx::begin() {
	//if (ENABLE_FxChain)
	{
		bArbPRE = ofGetUsingArbTex();
		ofDisableArbTex();

		fbo_FxChain.begin();
		ofClear(0, 0, 0, 0);
	}
}

//--------------------------------------------------------------
void ofxChannelFx::end() {
	//if (ENABLE_FxChain)
	{
		fbo_FxChain.end();

		if (bArbPRE) ofEnableArbTex();
		else ofDisableArbTex();
	}
}

//--------------------------------------------------------------
void ofxChannelFx::update_FxChannel()
{
	if (ENABLE_FxChain)
	{
		bArbPRE = ofGetUsingArbTex();
		ofDisableArbTex();

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

		if (bArbPRE) ofEnableArbTex();
		else ofDisableArbTex();
	}
}

//--------------------------------------------------------------
void ofxChannelFx::windowResized(int _w, int _h)
{
	window_W = _w;
	window_H = _h;

	//TODO:
	//resize fbo...
}

////--------------------------------------------------------------
//void ofxChannelFx::drawGui() {
//
//}

//--------------------------------------------------------------
void ofxChannelFx::draw()
{
	//if (ENABLE_FxChain)
	{
		bArbPRE = ofGetUsingArbTex();
		ofDisableArbTex();

		update_FxChannel();

		ofSetColor(255, 255, 255, 255);
		fbo_FxChain.draw(0, 0, window_W, window_H);

		if (bArbPRE) ofEnableArbTex();
		else ofDisableArbTex();
	}
}

#pragma mark - SETTINGS

//--------------------------------------------------------------
void ofxChannelFx::loadGroup(ofParameterGroup &g, string path)
{
	ofLogNotice(__FUNCTION__) << "loadGroup " << path;
	ofXml settings;
	settings.load(path);
	ofDeserialize(settings, g);
}

//--------------------------------------------------------------
void ofxChannelFx::saveGroup(ofParameterGroup &g, string path)
{
	ofLogNotice(__FUNCTION__) << "saveGroup " << path;
	ofXml settings;
	ofSerialize(settings, g);
	settings.save(path);
}

//--------------------------------------------------------------
void ofxChannelFx::setup_GuiTheme()
{
	//-

	//gui theme
	//must check before if the used font file is present
	string str = "overpass-mono-bold.otf";
	string pathFont = "assets/fonts/" + str;// /data/assets
	ofFile file(pathFont);
	bool b = file.exists();
	if (b)
	{
		ofLogNotice(__FUNCTION__) << pathFont << " FOUND";
	}
	else
	{
		ofLogError(__FUNCTION__) << pathFont << " NOT FOUND!";
	}
	if (b) {
		guiPanel->loadTheme("assets/theme/theme_ofxGuiExtended2.json");
	}

	//-

	//default
	guiPanel->setPosition(20, 20);

	//-

	//remove alpha slider from colors

	auto g0 = gFrag2->getGroup("COLOR 0");
	auto g1 = gFrag2->getGroup("COLOR 1");
	auto g2 = gFrag2->getGroup("COLOR 2");

	g0 = gFrag2->getGroup("COLOR 0");
	auto a0 = g0->getControl("a");
	a0->setHidden(true);
	g1 = gFrag2->getGroup("COLOR 1");
	auto a1 = g1->getControl("a");
	a1->setHidden(true);
	g2 = gFrag2->getGroup("COLOR 2");
	auto a2 = g2->getControl("a");
	a2->setHidden(true);

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
		{"height", 30},
		//{"text-align", "center"}
	};

	//j_TextBig =//not working bc are headers
	//{
	//	//{"fill-color", "rgba(128,128,128,0.4)" },
	//	//{"type", "fullsize"},
	//	{"height", 30},
	//	{"text-align", "center"},
	//	{"font-size	", 30}
	//};
	//
	//not working bc are headers..
	//gui_FxUser->setConfig(j_TextBig);
	//gui_FxEdit->setConfig(j_TextBig);
	//(gui_FxUser->getControl(SELECT_Fx_Name.getName()))->setConfig(j_TextBig);

	(guiGroup->getControl(ENABLE_FxChain.getName()))->setConfig(j_ButtonBig);
	(guiGroup->getControl(ENABLE_FxChain.getName()))->setConfig({ {"text-align", "center"} });

	//1. user
	//(gui_FxUser->getControl(SELECT_Fx_Name.getName()))->setConfig(j_SliderBig);
	//(gui_FxUser->getControl(SELECT_Fx_Name.getName()))->setShowName(false);
	(gui_FxUser->getControl(SELECT_Fx.getName()))->setConfig(j_SliderBig);
	//(gui_FxUser->getControl(SELECT_Solo.getName()))->setConfig(j_ButtonBig);
	//(gui_FxUser->getControl(RESET.getName()))->setConfig(j_ButtonBig);

	//2. editor
	(gui_FxUser->getControl(ENABLE_Monochrome.getName()))->setConfig(j_ButtonBig);
	(gui_FxUser->getControl(ENABLE_ThreeTones.getName()))->setConfig(j_ButtonBig);
	(gui_FxUser->getControl(ENABLE_HSB.getName()))->setConfig(j_ButtonBig);
#ifdef INCLUDE_FX_DELAYS
	(gui_FxUser->getControl(ENABLE_Delay.getName()))->setConfig(j_ButtonBig);
	(gui_FxUser->getControl(ENABLE_Echotrace.getName()))->setConfig(j_ButtonBig);
#endif

	//-
}

//--------------------------------------------------------------
void ofxChannelFx::refreshGuiCollapse_FxChannel() {
	ofLogNotice(__FUNCTION__);

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
		gFrag5->getMinimized() &&
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
}

//--------------------------------------------------------------
void ofxChannelFx::refreshGui_FxChannel()
{
	ofLogNotice(__FUNCTION__);

	//-

	//minimize all

	gFrag1->minimize();
	gFrag2->minimize();
	gFrag3->minimize();

#ifdef INCLUDE_FX_DELAYS
	gFrag4->minimize();
	gFrag5->minimize();
	//gFrag6->minimize();
#endif

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

	saveGroup(params_Session, path_GLOBAL_Folder + "/" + path_fileName_Session);

#ifndef INCLUDE_ofxPresetsManager
	saveGroup(params_Preset, path_GLOBAL_Folder + "/" + path_fileName_Preset);
#endif
}