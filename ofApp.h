/************************************************************
■現物合わせ : Design調整
・	StateChart : Flying -> ONでちょうどToshiba Ledが点灯し、AiFarm LedのFlashと重なるよう、d_eachState[STATE__FLYING] を調整する。
・	曲合わせ : 元曲で"ボーン"の時間 = t_Boon
	State = MagmaからONまでの時間を逆算し、TransitionTo_Magma() 内のsetpositionを変更する


■setup
・	最低限の照明状態で、WebCamSetting.appを起動し、設定値を作成する。
・	XBCamの検知に邪魔にならない範囲で、且つVisualもいい感じの逆光になるよう、Gui_Global->volLight_Back_max をset.
	XBaseの設置位置も同時に考える。

■Boot
・WebCamSetting.app : Panelを表示し、一旦defaultをsetし、再度、設定を読み込む
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
		STATE__FLYING,
		STATE__ON,
		STATE__ON_DIALOGUE,
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
		4000, // STATE__INTRO_RISE,
		1000, // STATE__INTRO_FALL,
		6000, // STATE__QUAKE_RISE,
		2000, // STATE__QUAKE_FALL,
		12000, // STATE__MAGMA,
		200, // STATE__DARK,
		800, // STATE__FLYING,
		20000, // STATE__ON,
		0, // STATE__ON_DIALOGUE,
		3000, // STATE__FADEOUT,
	};
	
	/****************************************
	****************************************/
	XBC_STATE XBC_State;
	COLOR_ID ColorId_of_Fire = COLOR_ID__BLACK;
	
	const double fps = 60.0;
	
	int LedId_Test = -1;
	
	STATE State = STATE__WAIT;
	
	bool b_skip = false;
	
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
		LED_PARAM(255, 255, 255, 0), // STATE__FLYING,
		LED_PARAM(255, 255, 255, 0), // STATE__ON,
		LED_PARAM(255, 255, 255, 0), // STATE__ON_DIALOGUE,
		LED_PARAM(255, 255, 255, 0), // STATE__FADEOUT,
	};
	
	ofTrueTypeFont font[NUM_FONTSIZE];
	
	/********************
	********************/
	ofSoundPlayer sound_Ambient;
	ofSoundPlayer sound_Mysterious;
	ofSoundPlayer sound_Quake;
	ofSoundPlayer sound_Magma;
	ofSoundPlayer sound_Thunder;
	ofSoundPlayer sound_Dooon;
	ofSoundPlayer sound_Fire;
	// ofSoundPlayer sound_Climax;
	
	SOUND_PAIR sound_Climax;
	
	/********************
	********************/
	ODE ode_Shutter = ODE("10.7.206.7");
	bool b_DmxShutter_Open = false;
	
	/********************
	********************/
	// VOLUME vol_Ligt_Dynamic;
	VOLUME vol_Light_Eye;
	VOLUME vol_Light_FaceUp;
	VOLUME vol_Light_FaceLow;
	VOLUME vol_Light_ArmUp;
	VOLUME vol_Light_ArmLow;
	
	VOLUME vol_Light_Climax;
	VOLUME vol_Light_Back;
	
	VOLUME vol_mov_Calm;
	VOLUME vol_mov_Evil;
	
	/********************
	********************/
	OSC_TARGET Osc_XBC;
	OSC_TARGET Osc_Video_Nebuta;
	
	/********************
	********************/
	int c_toFadeOut = 0;
	bool b_key_Enter = false;
	bool b_key_CheckLed = false;

	/****************************************
	****************************************/
	bool isFile_Exist(const char* FileName);
	void DmxOut_AllBlack();
	void setup_Gui();
	void SetFrontPattern__CheckLed(LED_LIGHT* _LedLight, int _NUM_LEDS);
	void SetFrontPattern__On(LED_LIGHT* _LedLight, int _NUM_LEDS);
	void SetFrontPattern__Off(LED_LIGHT* _LedLight, int _NUM_LEDS);
	void SetFrontPattern__Perlin(LED_LIGHT* _LedLight, int _NUM_LEDS);
	void SetFrontPattern__Strobe(LED_LIGHT* _LedLight, int _NUM_LEDS);
	void setup__RandomStrobe(ofx_LIGHTPATTERN* LightPattern, int now_ms, double L0, double L1, int NUM_LEDS);
	void SetBackPattern__Off(LED_LIGHT* _LedLight, int _NUM_LEDS);
	void SetBackPattern__1Time_Flash(LED_LIGHT* _LedLight, int _NUM_LEDS, int d_d);
	void TransitionTo_Wait();
	void TransitionTo_CheckLed();
	void TransitionTo_ManualOn();
	void TransitionTo_IntroRise();
	void TransitionTo_IntroFall();
	void TransitionTo_QuakeRise();
	void TransitionTo_QuakeFall();
	void TransitionTo_Magma();
	void TransitionTo_Dark();
	void TransitionTo_Flying();
	void TransitionTo_On();
	void TransitionTo_OnDialogue();
	void TransitionTo_FadeOut();
	void ReceiveOsc_from_XBC();
	void DmxShutter_open();
	void DmxShutter_close();
	void draw_info();
	void sendDmx_Light();
	void sendDmx_Shutter(bool b_open);
	void SendOSC();
	void SendOSC_Clear();
	void StateChart();
	void VolumeControl();
	void LoadAndSet_sounds();
	void setup_sound(ofSoundPlayer& sound, string FileName, bool b_Loop, float vol);
	void sound_VolUp_AutoStart(ofSoundPlayer& sound, double step, double limit = 1.0);
	void sound_VolDown_AutoStop(ofSoundPlayer& sound, double step, double limit = 0.0);
	void update_ColorOfFire();
	LED_PARAM get_ColorofFile();
	double my_SinWave(double T, double theta, double _min, double _max);
	void volLightDynamic__set(double val);
	void volLightDynamic__Down(double step, double limit = 0.0);
	void volLightDynamic__Up(double step, double limit = 1.0);
	bool volLightDynamic__IsZero();
	
public:
	/****************************************
	****************************************/
	ofApp();
	~ofApp();
	
	void setup();
	void update();
	void draw();
	void exit();

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
