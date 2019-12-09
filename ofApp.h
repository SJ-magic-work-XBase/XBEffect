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
		STATE__FADE_TO_END,
		STATE__NOSOUND,
		STATE__ENDING,
		STATE__FADEOUT,
		
		NUM_STATE,
	};
	
	enum COLOR_ID{
		COLOR_ID__BLACK,
		COLOR_ID__CALM,
		COLOR_ID__EVIL,
		
		NUM__COLOR_ID,
	};
	
	enum{ FONT_S, FONT_M, FONT_L, FONT_LL, NUM_FONTSIZE, };
	
	enum{
		NUM_VOICES = 8,
	};
	
	/****************************************
	****************************************/
	int d_eachState[NUM__COLOR_ID][NUM_STATE] = {
		{ // COLOR_ID__BLACK
			0, // STATE__WAIT,
			0, // STATE__CHECK_LED,
			0, // STATE__MANUAL_ON,
			4000, // STATE__INTRO_RISE,
			1000, // STATE__INTRO_FALL,
			6000, // STATE__QUAKE_RISE,
			2000, // STATE__QUAKE_FALL,
			12000, // STATE__MAGMA,
			1350, // STATE__DARK,
			650, // STATE__FLYING,
			
			20000, // STATE__ON,
			30000, // STATE__ON_DIALOGUE,
			10000, // STATE__FADE_TO_END,
			1000, // STATE__NOSOUND,
			0, // STATE__ENDING,
			3000, // STATE__FADEOUT,
		},
		{ // COLOR_ID__CALM
			0, // STATE__WAIT,
			0, // STATE__CHECK_LED,
			0, // STATE__MANUAL_ON,
			4000, // STATE__INTRO_RISE,
			1000, // STATE__INTRO_FALL,
			6000, // STATE__QUAKE_RISE,
			2000, // STATE__QUAKE_FALL,
			12000, // STATE__MAGMA,
			1350, // STATE__DARK,
			650, // STATE__FLYING,
			
			5000, // STATE__ON,
			70000, // STATE__ON_DIALOGUE,
			12000, // STATE__FADE_TO_END,
			2000, // STATE__NOSOUND,
			0, // STATE__ENDING,
			3000, // STATE__FADEOUT,
		},
		{ // COLOR_ID__EVIL
			0, // STATE__WAIT,
			0, // STATE__CHECK_LED,
			0, // STATE__MANUAL_ON,
			4000, // STATE__INTRO_RISE,
			1000, // STATE__INTRO_FALL,
			6000, // STATE__QUAKE_RISE,
			2000, // STATE__QUAKE_FALL,
			12000, // STATE__MAGMA,
			1350, // STATE__DARK,
			650, // STATE__FLYING,
			
			20000, // STATE__ON,
			56500, // STATE__ON_DIALOGUE,
			10000, // STATE__FADE_TO_END,
			3000, // STATE__NOSOUND,
			0, // STATE__ENDING,
			3000, // STATE__FADEOUT,
		},
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
	
	ofTrueTypeFont font[NUM_FONTSIZE];
	
	/********************
	********************/
	float LastVol_of_Sound = 0;
	
	ofSoundPlayer sound_Ambient;
	ofSoundPlayer sound_Mysterious;
	ofSoundPlayer sound_Quake;
	ofSoundPlayer sound_Magma;
	ofSoundPlayer sound_Thunder;
	ofSoundPlayer sound_Dooon;
	ofSoundPlayer sound_Fire;
	
	SOUND_PAIR sound_Climax;
	SOUND_PAIR sound_Ending;
	SOUND_PAIR sound_KimeEffect;
	
	/********************
	********************/
	SOUND_PAIR NebutaVoice[NUM_VOICES] = {
		SOUND_PAIR("_SelectedSound/Kira/GOOD_END_0.wav", "_SelectedSound/Kira/BAD_END_0.wav", 0, 0, false, 1.0),
		SOUND_PAIR("_SelectedSound/Kira/GOOD_END_1.wav", "_SelectedSound/Kira/BAD_END_1.wav", 0, 0, false, 1.0),
		SOUND_PAIR("_SelectedSound/Kira/GOOD_END_2.wav", "_SelectedSound/Kira/BAD_END_2.wav", 0, 0, false, 1.0),
		SOUND_PAIR("_SelectedSound/Kira/GOOD_END_3.wav", "_SelectedSound/Kira/BAD_END_3.wav", 0, 0, false, 1.0),
		SOUND_PAIR("_SelectedSound/Kira/GOOD_END_4.wav", "_SelectedSound/Kira/BAD_END_4.wav", 0, 0, false, 1.0),
		SOUND_PAIR("_SelectedSound/Kira/GOOD_END_5.wav", "_SelectedSound/Kira/BAD_END_5.wav", 0, 0, false, 1.0),
		SOUND_PAIR("_SelectedSound/Kira/GOOD_END_6.wav", "_SelectedSound/Kira/BAD_END_6.wav", 0, 0, false, 1.0),
		SOUND_PAIR("_SelectedSound/Kira/GOOD_END_7.wav", "_SelectedSound/Kira/BAD_END_7.wav", 0, 0, false, 1.0),
	};
	int t_NebutaVoice_Zero = 0;
	int NebutaVoiceId;
	bool b_NebutaVoiceEnd;
	
	/* */
	int t_NebutaVoiceStart_Evil[NUM_VOICES] = {				//		duration	space
			2000	,	//	「有象無象がおる…」（吐き捨てる）	4000	1000
			44000	,	//	「何も 知らず 同調し、己の ない　者ども」	9000	1000
	//		54000	,	//	「悪霊よ、滅びよ！」	2000	1000
			57000	,	//	「悪霊だと？」	3000	1000
			61000	,	//	「フフフ…またしても我を悪霊と呼ぶのか」	9000	500
			70500	,	//	「貴様たちが「悪」のレッテル貼りを続ける限り、我は滅びぬ。」	9000	500
			80000	,	//	「悪霊たる我を産んだのは…貴様たち自身だ…。」	7000	500
			87500	,	//	「…ゆめゆめ、忘れるな。この次は…コノ次ハ…‼」	10000	1000
			98500	,	//	「グワ″ーーーーッッ‼」	10000	1000
	};	//	109500					
								
								
	int t_NebutaVoiceStart_Calm[NUM_VOICES] = {							
			2000	,	//	「心地よい光だ…」	4000	2000
			29000	,	//	「魂が…洗い清められてゆく」	6000	1000
	//		36000	,	//	「鎮まれ。百鬼夜行と共に」	2000	2000
			40000	,	//	「百鬼夜行だと？」	4000	1000
			45000	,	//	「お前の眼には、アレがそう映ったというのか」	7000	1000
			53000	,	//	「ある者が見れば魑魅魍魎の百鬼夜行。だがある者が見れば、美しい光の行軍」	13000	1000
			67000	,	//	「善か？悪か？月か？太陽か？決めるのは、お前たち自身だ…。」	15000	1000
			83000	,	//	「曇りのない眼で、いつか再び会いに来い。楽しみにしているぞ」	12000	2000
			97000	,	//	「ワッハッハッハ…‼」	11000	1000
	};	//	109000					
	
	/********************
	********************/
	// ODE ode_Shutter = ODE("10.7.206.7");
	bool b_DmxShutter_Open = false;
	
	/********************
	********************/
	// VOLUME vol_Ligt_Dynamic;
	VOLUME vol_Light_Intro;
	VOLUME vol_Light_QuakeH;
	VOLUME vol_Light_Face;
	
	VOLUME vol_Light_Climax;
	VOLUME vol_Light_Back;
	VOLUME vol_Light_Vol;
	
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
	float Cal_VolOfSound();
	bool isFile_Exist(const char* FileName);
	void DmxOut_AllBlack();
	void setup_Gui();
	void SetFrontPattern__CheckLed(LED_LIGHT* _LedLight, int _NUM_LEDS);
	void SetFrontPattern__On(LED_LIGHT* _LedLight, int _NUM_LEDS);
	void SetFrontPattern__On(LED_LIGHT* _LedLight, const int* id_list);
	void SetFrontPattern__Off(LED_LIGHT* _LedLight, int _NUM_LEDS);
	void SetFrontPattern__Off(LED_LIGHT* _LedLight, const int* id_list);
	void SetFrontPattern__1Time_Flash(LED_LIGHT* _LedLight, int _NUM_LEDS, int d_d);
	void SetFrontPattern__1Time_Flash(LED_LIGHT* _LedLight, const int* id_list, int d_d);
	void SetFrontPattern__Perlin(LED_LIGHT* _LedLight, int _NUM_LEDS);
	void SetFrontPattern__Perlin(LED_LIGHT* _LedLight, const int* id_list);
	void SetFrontPattern__Strobe_magma(LED_LIGHT* _LedLight, int _NUM_LEDS);
	void SetFrontPattern__Strobe_magma(LED_LIGHT* _LedLight, const int* id_list);
	void SetFrontPattern__Strobe_FadeToEnd(LED_LIGHT* _LedLight, int _NUM_LEDS);
	void SetFrontPattern__Strobe_FadeToEnd(LED_LIGHT* _LedLight, const int* id_list);
	void setup__RandomStrobe_magma(ofx_LIGHTPATTERN* LightPattern, int now_ms, double L0, double L1, int NUM_LEDS);
	void setup__RandomStrobe_FadeToEnd(ofx_LIGHTPATTERN* LightPattern, int now_ms, double L0, double L1, int NUM_LEDS);
	void SetBackPattern__Off(LED_LIGHT* _LedLight, int _NUM_LEDS);
	void SetBackPattern__Off(LED_LIGHT* _LedLight, const int* id_list);
	void SetBackPattern__1Time_Flash(LED_LIGHT* _LedLight, int _NUM_LEDS, int d_d);
	void SetBackPattern__1Time_Flash(LED_LIGHT* _LedLight, const int* id_list, int d_d);
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
	void TransitionTo_FadeToEnd();
	void TransitionTo_NoSound();
	void TransitionTo_Ending();
	void TransitionTo_FadeOut();
	void ReceiveOsc_from_XBC();
	void DmxShutter_open();
	void DmxShutter_close();
	void draw_info();
	void sendDmx();
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
	void ControlNebutaVoice();

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
