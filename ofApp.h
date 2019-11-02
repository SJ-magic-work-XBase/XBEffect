/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "ofxOsc_BiDirection.h"
#include "ofxArtnet.h"
#include "ofxSetLightPattern.h"

#include "sj_common.h"
#include "util.h"
#include "Light.h"

/************************************************************
************************************************************/


/**************************************************
**************************************************/
class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	enum STATE{
		STATE__WAIT,
		STATE__CHECK_LED,
		STATE__MANUAL_ON,
		STATE__INTRO_RISE,
		STATE__INTRO_FALL,
		STATE__QUAKE_RISE,
		STATE__QUAKE_FALL,
		STATE__MAGMA,
		STATE__DARK,
		STATE__ON,
		STATE__FADEOUT,
		
		NUM_STATE,
	};
	
	enum COLOR_ID{
		COLOR_ID__BLACK,
		COLOR_ID__CALM,
		COLOR_ID__EVIL,
	};
	
	enum{ FONT_S, FONT_M, FONT_L, FONT_LL, NUM_FONTSIZE, };
	
	/****************************************
	****************************************/
	int d_eachState[NUM_STATE] = {
		0, // STATE__WAIT,
		0, // STATE__CHECK_LED,
		0, // STATE__MANUAL_ON,
		3000, // STATE__INTRO_RISE,
		1000, // STATE__INTRO_FALL,
		6000, // STATE__QUAKE_RISE,
		1000, // STATE__QUAKE_FALL,
		12000, // STATE__MAGMA,
		1000, // STATE__DARK,
		0, // STATE__ON,
		2000, // STATE__FADEOUT,
	};
	
	/****************************************
	****************************************/
	XBC_STATE XBC_State;
	COLOR_ID ColorId_of_Fire = COLOR_ID__BLACK;
	
	const double fps = 60.0;
	
	int LedId_Test = -1;
	
	STATE State = STATE__WAIT;
	
	int now = 0;
	int t_Last = 0;
	int t_from = 0;
	
	const LED_PARAM LedCol_ofTheState[NUM_STATE] = {
		LED_PARAM(255, 255, 255, 0), // STATE__WAIT,
		LED_PARAM(255, 255, 255, 0), // STATE__CHECK_LED,
		LED_PARAM(255, 255, 255, 0), // STATE__MANUAL_ON,
		LED_PARAM(255, 255, 255, 0), // STATE__INTRO_RISE,
		LED_PARAM(255, 255, 255, 0), // STATE__INTRO_FALL,
		LED_PARAM(255, 255, 255, 0), // STATE__QUAKE_RISE,
		LED_PARAM(255, 255, 255, 0), // STATE__QUAKE_FALL,
		LED_PARAM(255, 255, 255, 0), // STATE__MAGMA,
		LED_PARAM(255, 255, 255, 0), // STATE__DARK,
		LED_PARAM(255, 255, 255, 0), // STATE__ON,
		LED_PARAM(255, 255, 255, 0), // STATE__FADEOUT,
	};
	
	ofTrueTypeFont font[NUM_FONTSIZE];
	
	/********************
	********************/
	ofSoundPlayer sound_Mysterious;
	ofSoundPlayer sound_Quake;
	ofSoundPlayer sound_Magma;
	ofSoundPlayer sound_Thunder;
	ofSoundPlayer sound_Dooon;
	
	/********************
	********************/
	VOLUME vol_Ligt;
	
	/********************
	********************/
	OSC_TARGET Osc_XBC;
	OSC_TARGET Osc_Video_Nebuta;
	OSC_TARGET Osc_Video_Wall;
	
	/********************
	********************/
	bool b_key_Enter = false;
	bool b_key_CheckLed = false;

	/****************************************
	****************************************/
	void DmxOut_AllBlack();
	void setup_Gui();
	void SetLightPattern__CheckLed();
	void SetLightPattern__On();
	void SetLightPattern__Perlin();
	void SetLightPattern__Strobe();
	void setup__RandomStrobe(ofx_LIGHTPATTERN* LightPattern, int now_ms, double L0, double L1);
	void SetLightPattern_Back__Off();
	void SetLightPattern_Back__1Time_Flash();
	void TransitionTo_Wait();
	void TransitionTo_CheckLed();
	void TransitionTo_ManualOn();
	void TransitionTo_IntroRise();
	void TransitionTo_IntroFall();
	void TransitionTo_QuakeRise();
	void TransitionTo_QuakeFall();
	void TransitionTo_Magma();
	void TransitionTo_Dark();
	void TransitionTo_On();
	void TransitionTo_FadeOut();
	void ReceiveOsc_from_XBC();
	void DmxShutter_open();
	void DmxShutter_close();
	void draw_info();
	void sendDmx();
	void SendOSC();
	void StateChart();
	void VolumeControl();
	void LoadAndSet_sounds();
	void setup_sound(ofSoundPlayer& sound, string FileName, bool b_Loop, float vol);
	void sound_VolUp_AutoStart(ofSoundPlayer& sound, double step, double limit = 1.0);
	void sound_VolDown_AutoStop(ofSoundPlayer& sound, double step, double limit = 0.0);
	void update_ColorOfFire();
	LED_PARAM get_ColorofFile();
	
public:
	/****************************************
	****************************************/
	ofApp();
	~ofApp();
	
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
};
