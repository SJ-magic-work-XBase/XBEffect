/************************************************************
************************************************************/
#include "ofApp.h"

/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp()
: Osc_XBC("127.0.0.1", 12346, 12345)
, Osc_Video_Nebuta("127.0.0.1", 12348, 12347)

, sound_Climax("_SelectedSound/Climax_Calm/ssm-102518-V.Bellini-Norma-Cavatina-di-Norma.wav", "_SelectedSound/Climax_Evil/OVCL-00123 Track24-25  (96kHz-24bit).wav", 276000, 112200, false, 0.0)
, sound_Ending("_SelectedSound/End/the-enemy-awaits_zJR6kQr_.wav", "_SelectedSound/End/heartbeat-scary-music_GkOY9NN_.wav", 40000, 0, false, 0.0)
, sound_KimeEffect("_SelectedSound/AtKime/fireball-whoosh_GknWsSNu.wav", "_SelectedSound/AtKime/explosion-blast-large-bass-rumble_fk5jCFVu.wav", 0, 0, false, 0.0)
{
	/********************
	********************/
	fp_Log = fopen("../../../Log.csv", "w");
	if(fp_Log == nullptr){
		printf("Log file open NG\n");
		fflush(stdout);
		std::exit(1);
	}
	
	/********************
	********************/
	font[FONT_S].load("font/RictyDiminished-Regular.ttf", 10, true, true, true);
	font[FONT_M].load("font/RictyDiminished-Regular.ttf", 12, true, true, true);
	font[FONT_L].load("font/RictyDiminished-Regular.ttf", 15, true, true, true);
	font[FONT_LL].load("font/RictyDiminished-Regular.ttf", 30, true, true, true);
}

/******************************
******************************/
ofApp::~ofApp()
{
	if(fp_Log) fclose(fp_Log);
	if(Gui_Global) delete Gui_Global;
	
	DmxOut_AllBlack();
}

/******************************
******************************/
void ofApp::exit()
{
	SendOSC_Clear();
}

/******************************
******************************/
void ofApp::DmxOut_AllBlack(){
	for(int i = 0; i < NUM_ODES; i++){
		ode[i].ClearUniverse();
		ode[i].SendDmx();
	}
}

/******************************
******************************/
void ofApp::setup(){
	/********************
	********************/
	ofSetWindowTitle("XB Effect");
	
	ofSetWindowShape(WINDOW_WIDTH, WINDOW_HEIGHT);
	/*
	ofSetVerticalSync(false);
	ofSetFrameRate(0);
	/*/
	ofSetVerticalSync(true);
	ofSetFrameRate(fps);
    //*/
	
	ofSetEscapeQuitsApp(false);
	
	/********************
	********************/
	setup_Gui();
	if(isFile_Exist("../../../data/gui.xml"))	Gui_Global->gui.loadFromFile("gui.xml");
	
	SetFrontPattern__On(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
	SetBackPattern__Off(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
	SetFrontPattern__On(Light_Climax, NUM_LIGHTS_CLIMAX);
	SetBackPattern__Off(Light_Climax, NUM_LIGHTS_CLIMAX);
	SetFrontPattern__On(Light_Back, NUM_LIGHTS_BACK);
	SetBackPattern__Off(Light_Back, NUM_LIGHTS_BACK);
	
	LoadAndSet_sounds();
	
	/********************
	********************/
#ifdef ARTNET_THREADED
	artnet.enableThread(fps);	// main windowと同じにしておくのが安全かな.
#endif
}

/******************************
******************************/
bool ofApp::isFile_Exist(const char* FileName)
{
	FILE* fp = fopen(FileName, "r");
	if(fp == nullptr){
		return false;
	}else{
		fclose(fp);
		
		printf(">>> gui : load from file <<<\n");
		fflush(stdout);
		
		return true;
	}
}

/******************************
******************************/
void ofApp::LoadAndSet_sounds()
{
	setup_sound(sound_Ambient, "_SelectedSound/jg-032316-sfx-windy-planet-ambiance-4.wav", true, 0.0);
	setup_sound(sound_Mysterious, "_SelectedSound/mysterious-dungeons-ambiance_GkvZUHVO.wav", true, 0.0);
	setup_sound(sound_Quake, "_SelectedSound/lava-flowing_f1gkErVd.wav", true, 0.0);
	setup_sound(sound_Magma, "_SelectedSound/flowing-magma-foley_MJC0mBNu.wav", true, 0.0);
	setup_sound(sound_Thunder, "_SelectedSound/thunder-clap_z19asnEO.wav", false, 1.0);
	setup_sound(sound_Dooon, "_SelectedSound/explosion-blast-large-bass-rumble_fk5jCFVu.wav", false, 1.0);
	setup_sound(sound_Fire, "_SelectedSound/ab-f-031616-38568456_bonfire-burning-ambience-01.wav", true, 0.0);
}

/******************************
******************************/
void ofApp::setup_sound(ofSoundPlayer& sound, string FileName, bool b_Loop, float vol)
{
	sound.load(FileName.c_str());
	if(sound.isLoaded()){
		sound.setLoop(b_Loop);
		sound.setMultiPlay(true);
		sound.setVolume(vol);
	}else{
		printf("%s load Error\n", FileName.c_str());
		fflush(stdout);
	}
}

/******************************
******************************/
void ofApp::sound_VolUp_AutoStart(ofSoundPlayer& sound, double step, double limit)
{
	if(!sound.isLoaded()) return;
	
	if(!sound.isPlaying()){
		sound.play();
		sound.setPosition(0);
	}
	
	float vol = sound.getVolume();
	vol += step;
	if(limit < vol) vol = limit;
	sound.setVolume(vol);
}

/******************************
******************************/
void ofApp::sound_VolDown_AutoStop(ofSoundPlayer& sound, double step, double limit)
{
	if(!sound.isLoaded()) return;
	
	float vol = sound.getVolume();
	vol -= step;
	if(vol < limit) vol = limit;
	sound.setVolume(vol);
	
	if( (vol <= 0) && (sound.isPlaying()) ){
		sound.stop();
		sound.setPosition(0);
	}
}

/******************************
description
	memoryを確保は、app start後にしないと、
	segmentation faultになってしまった。
******************************/
void ofApp::setup_Gui()
{
	/********************
	********************/
	Gui_Global = new GUI_GLOBAL;
	Gui_Global->setup("XBE", "gui.xml", 10, 10);
}

/******************************
******************************/
void ofApp::SetFrontPattern__CheckLed(LED_LIGHT* _LedLight, int _NUM_LEDS)
{
	for(int i = 0; i < _NUM_LEDS; i++){
		if(i == LedId_Test)	_LedLight[i].LightPattern_Front.setup(now, 1.0);
		else				_LedLight[i].LightPattern_Front.setup(now, 0.0);
	}
}

/******************************
******************************/
void ofApp::SetFrontPattern__On(LED_LIGHT* _LedLight, int _NUM_LEDS)
{
	for(int i = 0; i < _NUM_LEDS; i++){
		_LedLight[i].LightPattern_Front.setup(now, 1.0);
	}
}

/******************************
******************************/
void ofApp::SetFrontPattern__On(LED_LIGHT* _LedLight, const int* id_list)
{
	for(int i = 0; id_list[i] != -1; i++){
		_LedLight[id_list[i]].LightPattern_Front.setup(now, 1.0);
	}
}

/******************************
******************************/
void ofApp::SetFrontPattern__Off(LED_LIGHT* _LedLight, int _NUM_LEDS)
{
	for(int i = 0; i < _NUM_LEDS; i++){
		_LedLight[i].LightPattern_Front.setup(now, 0.0);
	}
}

/******************************
******************************/
void ofApp::SetFrontPattern__Off(LED_LIGHT* _LedLight, const int* id_list)
{
	for(int i = 0; id_list[i] != -1; i++){
		_LedLight[id_list[i]].LightPattern_Front.setup(now, 0.0);
	}
}

/******************************
******************************/
void ofApp::SetFrontPattern__1Time_Flash(LED_LIGHT* _LedLight, int _NUM_LEDS, int d_d)
{
	for(int i = 0; i < _NUM_LEDS; i++){
		ofx_SET_LIGHTPATTERN::setup__1Time_Flash(&_LedLight[i].LightPattern_Front, now, 0.0, 1.0, d_d);
	}
}

/******************************
******************************/
void ofApp::SetFrontPattern__1Time_Flash(LED_LIGHT* _LedLight, const int* id_list, int d_d)
{
	for(int i = 0; id_list[i] != -1; i++){
		ofx_SET_LIGHTPATTERN::setup__1Time_Flash(&_LedLight[id_list[i]].LightPattern_Front, now, 0.0, 1.0, d_d);
	}
}

/******************************
******************************/
void ofApp::SetFrontPattern__Perlin(LED_LIGHT* _LedLight, int _NUM_LEDS)
{
	for(int i = 0; i < _NUM_LEDS; i++){
		// ofx_SET_LIGHTPATTERN::setup__Perlin(&LedLight[i].LightPattern_Front, now, 0, 1.0, 2000);
		ofx_SET_LIGHTPATTERN::setup__Perlin(&_LedLight[i].LightPattern_Front, now, -0.5, 1.5, 1000);
	}
}

/******************************
******************************/
void ofApp::SetFrontPattern__Perlin(LED_LIGHT* _LedLight, const int* id_list)
{
	for(int i = 0; id_list[i] != -1; i++){
		ofx_SET_LIGHTPATTERN::setup__Perlin(&_LedLight[id_list[i]].LightPattern_Front, now, -0.5, 1.5, 1000);
	}
}

/******************************
******************************/
void ofApp::SetFrontPattern__Strobe_magma(LED_LIGHT* _LedLight, int _NUM_LEDS)
{
	for(int i = 0; i < _NUM_LEDS; i++){
		setup__RandomStrobe_magma(&_LedLight[i].LightPattern_Front, now, 0.0, 1.0, _NUM_LEDS);
	}
}

/******************************
******************************/
void ofApp::SetFrontPattern__Strobe_magma(LED_LIGHT* _LedLight, const int* id_list)
{
	int _NUM_LEDS = 0;
	for(_NUM_LEDS = 0; id_list[_NUM_LEDS] != -1; _NUM_LEDS++) {/* just count */}
	
	for(int i = 0; id_list[i] != -1; i++){
		setup__RandomStrobe_magma(&_LedLight[id_list[i]].LightPattern_Front, now, 0.0, 1.0, _NUM_LEDS);
	}
}

/******************************
******************************/
void ofApp::SetFrontPattern__Strobe_FadeToEnd(LED_LIGHT* _LedLight, int _NUM_LEDS)
{
	for(int i = 0; i < _NUM_LEDS; i++){
		setup__RandomStrobe_FadeToEnd(&_LedLight[i].LightPattern_Front, now, 0.0, 1.0, _NUM_LEDS);
	}
}

/******************************
******************************/
void ofApp::SetFrontPattern__Strobe_FadeToEnd(LED_LIGHT* _LedLight, const int* id_list)
{
	int _NUM_LEDS = 0;
	for(_NUM_LEDS = 0; id_list[_NUM_LEDS] != -1; _NUM_LEDS++) {/* just count */}
	
	for(int i = 0; id_list[i] != -1; i++){
		setup__RandomStrobe_FadeToEnd(&_LedLight[id_list[i]].LightPattern_Front, now, 0.0, 1.0, _NUM_LEDS);
	}
}

/******************************
******************************/
void ofApp::setup__RandomStrobe_magma(ofx_LIGHTPATTERN* LightPattern, int now_ms, double L0, double L1, int NUM_LEDS)
{
	/********************
	********************/
	int d_d_ms = 33; // 1 strobeの鋭さ
	int d_Transition_T_ms = d_eachState[ColorId_of_Fire][STATE__MAGMA] * 1/2; // 遷移時間
	
	/*
	どの時間を見ても1個のLedが光っている(実際はRange内でRandom selectなので、期待値)
		T = d_d_ms * NUM_LEDS / 1;
		
	どの時間を見てもX個のLedが光っている(期待値)
		T = d_d_ms * NUM_LEDS / X;
	
	常に、全てのLedが光っている(期待値)
		T = d_d_ms * NUM_LEDS / NUM_LEDS;
	*/
	ofx_LIGHTPATTERN::MIN_MAX_PAIR T_from( d_d_ms * NUM_LEDS/(max(NUM_LEDS/10, 1)), d_d_ms * NUM_LEDS/(max(NUM_LEDS/12, 1)) );
	ofx_LIGHTPATTERN::MIN_MAX_PAIR T_to( d_d_ms * NUM_LEDS/(max(NUM_LEDS/2, 1)), d_d_ms * NUM_LEDS/(max(NUM_LEDS/5, 1)) );
	
	/* */
	LightPattern->setup(now_ms, L0, L1, 0, 0, d_d_ms, T_from, T_to, d_Transition_T_ms);
}

/******************************
******************************/
void ofApp::setup__RandomStrobe_FadeToEnd(ofx_LIGHTPATTERN* LightPattern, int now_ms, double L0, double L1, int NUM_LEDS)
{
	/********************
	********************/
	int d_d_ms = 33; // 1 strobeの鋭さ
	int d_Transition_T_ms = d_eachState[ColorId_of_Fire][STATE__FADE_TO_END]; // 遷移時間
	
	/*
	どの時間を見ても1個のLedが光っている(実際はRange内でRandom selectなので、期待値)
		T = d_d_ms * NUM_LEDS / 1;
		
	どの時間を見てもX個のLedが光っている(期待値)
		T = d_d_ms * NUM_LEDS / X;
	
	常に、全てのLedが光っている(期待値)
		T = d_d_ms * NUM_LEDS / NUM_LEDS;
	*/
	ofx_LIGHTPATTERN::MIN_MAX_PAIR T_from( d_d_ms * NUM_LEDS/(max(NUM_LEDS/2, 1)), d_d_ms * NUM_LEDS/(max(NUM_LEDS/5, 1)) );
	ofx_LIGHTPATTERN::MIN_MAX_PAIR T_to( d_d_ms * NUM_LEDS/(max(NUM_LEDS/10, 1)), d_d_ms * NUM_LEDS/(max(NUM_LEDS/12, 1)) );
	
	/* */
	LightPattern->setup(now_ms, L0, L1, 0, 0, d_d_ms, T_from, T_to, d_Transition_T_ms);
}

/******************************
******************************/
void ofApp::SetBackPattern__Off(LED_LIGHT* _LedLight, int _NUM_LEDS)
{
	for(int i = 0; i < _NUM_LEDS; i++){
		_LedLight[i].LightPattern_Back.setup(now, 0.0);
	}
}

/******************************
******************************/
void ofApp::SetBackPattern__Off(LED_LIGHT* _LedLight, const int* id_list)
{
	for(int i = 0; id_list[i] != -1; i++){
		_LedLight[id_list[i]].LightPattern_Back.setup(now, 0.0);
	}
}

/******************************
******************************/
void ofApp::SetBackPattern__1Time_Flash(LED_LIGHT* _LedLight, int _NUM_LEDS, int d_d)
{
	for(int i = 0; i < _NUM_LEDS; i++){
		ofx_SET_LIGHTPATTERN::setup__1Time_Flash(&_LedLight[i].LightPattern_Back, now, 0.0, 1.0, d_d);
	}
}

/******************************
******************************/
void ofApp::SetBackPattern__1Time_Flash(LED_LIGHT* _LedLight, const int* id_list, int d_d)
{
	for(int i = 0; id_list[i] != -1; i++){
		ofx_SET_LIGHTPATTERN::setup__1Time_Flash(&_LedLight[id_list[i]].LightPattern_Back, now, 0.0, 1.0, d_d);
	}
}

/******************************
******************************/
void ofApp::DmxShutter_open()
{
	b_DmxShutter_Open = true;
}

/******************************
******************************/
void ofApp::DmxShutter_close()
{
	b_DmxShutter_Open = false;
}

/******************************
******************************/
void ofApp::ReceiveOsc_from_XBC(){
	while(Osc_XBC.OscReceive.hasWaitingMessages()){
		ofxOscMessage m_receive;
		Osc_XBC.OscReceive.getNextMessage(m_receive); // 引数がpointerから参照に変わった.
		
		if(m_receive.getAddress() == "/XBCam"){
			bool b_Calm, b_Evil, b_Motion;
			
			if(m_receive.getArgAsFloat(0) == 1.0)	b_Evil = true;
			else									b_Evil = false;
			
			if(m_receive.getArgAsFloat(1) == 1.0)	b_Calm = true;
			else									b_Calm = false;
			
			if(m_receive.getArgAsFloat(2) == 1.0)	b_Motion = true;
			else									b_Motion = false;
			
			XBC_State.update_osc(now, b_Calm, b_Evil, b_Motion);
		}
	}	
}

/******************************
******************************/
void ofApp::TransitionTo_Wait(){
	State = STATE__WAIT;
	t_from = now;
	
	SetBackPattern__Off(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
	
	DmxShutter_close();
	
	ColorId_of_Fire = COLOR_ID__BLACK;
}

/******************************
******************************/
void ofApp::TransitionTo_CheckLed(){
	State = STATE__CHECK_LED;
	t_from = now;
	
	SetFrontPattern__CheckLed(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
	SetBackPattern__Off(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
	LedId_Test = -1;
	
	volLightDynamic__set(1.0);
	vol_Light_Climax.set(0.0);
	vol_Light_Back.set(0.0);
}

/******************************
******************************/
void ofApp::TransitionTo_ManualOn(){
	State = STATE__MANUAL_ON;
	t_from = now;
	
	DmxShutter_open();
	
	SetFrontPattern__On(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
	SetBackPattern__Off(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
	
	volLightDynamic__set(0.0);
	vol_Light_Climax.set(1.0);
	vol_Light_Back.set(0.0);
}

/******************************
******************************/
void ofApp::TransitionTo_IntroRise(){
	State = STATE__INTRO_RISE;
	t_from = now;
	
	DmxShutter_open();
	
	update_ColorOfFire();
}

/******************************
******************************/
void ofApp::TransitionTo_IntroFall(){
	State = STATE__INTRO_FALL;
	t_from = now;
}

/******************************
******************************/
void ofApp::TransitionTo_QuakeRise(){
	State = STATE__QUAKE_RISE;
	t_from = now;
	
	DmxShutter_open();
	
	/* */
	SetFrontPattern__Perlin(Light_Dynamic, id_Intro);
	SetFrontPattern__Perlin(Light_Dynamic, id_QuakeH);
	SetFrontPattern__On(Light_Dynamic, id_Face);
	
	SetBackPattern__Off(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
	
	/* */
	update_ColorOfFire();
	b_skip = true;
	
	/********************
	********************/
	t_NebutaVoice_Zero = now;
	NebutaVoiceId = 0;
	b_NebutaVoiceEnd = false;
}

/******************************
******************************/
void ofApp::TransitionTo_QuakeFall(){
	State = STATE__QUAKE_FALL;
	t_from = now;
}

/******************************
******************************/
void ofApp::TransitionTo_Magma(){
	State = STATE__MAGMA;
	t_from = now;
	
	sound_Thunder.play();
	
	/* */
	SetFrontPattern__Strobe_magma(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
	SetBackPattern__1Time_Flash(Light_Dynamic, NUM_LIGHTS_DYNAMIC, 600);	
	
	/**/
	volLightDynamic__set(1.0);
	// vol_Light_Climax.set(0.0);
	// vol_Light_Back.set(0.0);
	
	update_ColorOfFire();
	b_skip = false;
	
	if(sound_Climax.isPlaying()) sound_Climax.stop();
	sound_Climax.play(ColorId_of_Fire == COLOR_ID__CALM);
	sound_Climax.setVolume(0);
	sound_Climax.setStartPositionMS();
}

/******************************
******************************/
void ofApp::TransitionTo_Dark(){
	State = STATE__DARK;
	t_from = now;
	
	SetFrontPattern__On(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
	SetBackPattern__Off(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
	
	volLightDynamic__set(0.0);
	vol_Light_Climax.set(0.0);
	// vol_Light_Back.set(0.0);
}

/******************************
******************************/
void ofApp::TransitionTo_Flying(){
	State = STATE__FLYING;
	t_from = now;
	
	SetFrontPattern__On(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
	SetBackPattern__Off(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
	
	vol_Light_Climax.set(1.0);
}

/******************************
******************************/
void ofApp::TransitionTo_On(){
	State = STATE__ON;
	t_from = now;
	
	SetFrontPattern__Off(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
	SetBackPattern__1Time_Flash(Light_Dynamic, NUM_LIGHTS_DYNAMIC, 600);
	
	volLightDynamic__set(1.0);
	vol_Light_Climax.set(1.0);
	// vol_Light_Back.set(0.0);
	vol_Light_Vol.set(1.0);
	
	sound_Dooon.setVolume(1.0);
	sound_Dooon.setPosition(0);
	sound_Dooon.play();
	
	// update_ColorOfFire();	
}

/******************************
******************************/
void ofApp::TransitionTo_OnDialogue(){
	State = STATE__ON_DIALOGUE;
	t_from = now;
	
	SetFrontPattern__On(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
	SetBackPattern__Off(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
	
	volLightDynamic__set(0.0);
	vol_Light_Climax.set(1.0);
	// vol_Light_Back.set(0.0);
}

/******************************
******************************/
void ofApp::TransitionTo_FadeToEnd(){
	State = STATE__FADE_TO_END;
	t_from = now;
	
	/* */
	SetFrontPattern__1Time_Flash(Light_Dynamic, NUM_LIGHTS_DYNAMIC, d_eachState[ColorId_of_Fire][STATE__FADE_TO_END] * 3/4);
	SetBackPattern__1Time_Flash(Light_Dynamic, NUM_LIGHTS_DYNAMIC, 600);
	
	SetFrontPattern__1Time_Flash(Light_Back, NUM_LIGHTS_BACK, 600);
	SetBackPattern__1Time_Flash(Light_Back, NUM_LIGHTS_BACK, 600);
	
	/* */
	volLightDynamic__set(1.0);
	// vol_Light_Climax.set(1.0);
	vol_Light_Back.set(1.0);
	
	/* */
	sound_KimeEffect.play(ColorId_of_Fire == COLOR_ID__CALM);
	sound_KimeEffect.setVolume(1.0);
	sound_KimeEffect.setStartPositionMS();
	
	NebutaVoice[NUM_VOICES - 1].play(ColorId_of_Fire == COLOR_ID__CALM);
	NebutaVoice[NUM_VOICES - 1].setVolume(1.0);
	NebutaVoice[NUM_VOICES - 1].setStartPositionMS();
}

/******************************
******************************/
void ofApp::TransitionTo_NoSound(){
	State = STATE__NOSOUND;
	t_from = now;
	
	/* */
	SetFrontPattern__On(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
	SetBackPattern__Off(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
	
	SetFrontPattern__On(Light_Back, NUM_LIGHTS_BACK);
	SetBackPattern__Off(Light_Back, NUM_LIGHTS_BACK);
	
	/* */
	volLightDynamic__set(0.0);
	vol_Light_Back.set(0.0);
}

/******************************
******************************/
void ofApp::TransitionTo_Ending(){
	State = STATE__ENDING;
	t_from = now;
	
	c_toFadeOut = 0;
	
	/* */
	vol_Light_Climax.set(1.0);
	
	/* */
	sound_Ending.play(ColorId_of_Fire == COLOR_ID__CALM);
	sound_Ending.setVolume(0);
	sound_Ending.setStartPositionMS();
}

/******************************
******************************/
void ofApp::TransitionTo_FadeOut(){
	State = STATE__FADEOUT;
	t_from = now;
	
	DmxShutter_close();
}

/******************************
******************************/
void ofApp::StateChart(){
	switch(State){
		case STATE__WAIT:
			if( b_skip && (15000 < now - t_from) )	b_skip = false;
			
			if(b_key_CheckLed)							TransitionTo_CheckLed();
			else if(b_key_Enter)						TransitionTo_ManualOn();
			else if(XBC_State.Is_LedOn(now) && b_skip)	TransitionTo_QuakeRise();
			else if(XBC_State.Is_LedOn(now) && !b_skip)	TransitionTo_IntroRise();
			break;
			
		case STATE__CHECK_LED:
			if(b_key_CheckLed){
				TransitionTo_Wait();
				
				/* 明示的にsetする必要あり */
				SetFrontPattern__On(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
				SetBackPattern__Off(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
				
				volLightDynamic__set(0.0);
				vol_Light_Climax.set(0.0);
				vol_Light_Back.set(0.0);
			}
			break;
			
		case STATE__MANUAL_ON:
			if(b_key_Enter){
				TransitionTo_Wait();
				
				/* 明示的にsetする必要あり */
				SetFrontPattern__On(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
				SetBackPattern__Off(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
				
				volLightDynamic__set(0.0);
				vol_Light_Climax.set(0.0);
				vol_Light_Back.set(0.0);
			}
			break;
			
		case STATE__INTRO_RISE:
			if(!XBC_State.Is_LedOn(now))				TransitionTo_Wait();
			else if(d_eachState[ColorId_of_Fire][State] < now - t_from)	TransitionTo_IntroFall();
			break;
			
		case STATE__INTRO_FALL:
			if(!XBC_State.Is_LedOn(now))				TransitionTo_Wait();
			else if(d_eachState[ColorId_of_Fire][State] < now - t_from)	TransitionTo_QuakeRise();
			break;
			
		case STATE__QUAKE_RISE:
			if(!XBC_State.Is_LedOn(now))				TransitionTo_Wait();
			else if(d_eachState[ColorId_of_Fire][State] < now - t_from)	TransitionTo_QuakeFall();
			break;
			
		case STATE__QUAKE_FALL:
			if(!XBC_State.Is_LedOn(now))				TransitionTo_Wait();
			else if(d_eachState[ColorId_of_Fire][State] < now - t_from)	TransitionTo_Magma();
			break;
			
		case STATE__MAGMA:
			if(d_eachState[ColorId_of_Fire][State] < now - t_from)		TransitionTo_Dark();
			break;
			
		case STATE__DARK:
			if(d_eachState[ColorId_of_Fire][State] < now - t_from)		TransitionTo_Flying();
			break;
			
		case STATE__FLYING:
			if(d_eachState[ColorId_of_Fire][State] < now - t_from)		TransitionTo_On();
			break;
			
		case STATE__ON:
			if(d_eachState[ColorId_of_Fire][State] < now - t_from)		TransitionTo_OnDialogue();
			break;
			
		case STATE__ON_DIALOGUE:
			if(d_eachState[ColorId_of_Fire][State] < now - t_from)		TransitionTo_FadeToEnd();
			break;
			
		case STATE__FADE_TO_END:
			if(d_eachState[ColorId_of_Fire][State] < now - t_from)		TransitionTo_NoSound();
			break;
			
		case STATE__NOSOUND:
			if(d_eachState[ColorId_of_Fire][State] < now - t_from)		TransitionTo_Ending();
			break;
			
		case STATE__ENDING:
			if(b_key_Enter)	c_toFadeOut++;
			if(2 <= c_toFadeOut)	TransitionTo_FadeOut();
			break;
			
		case STATE__FADEOUT:
			if((d_eachState[ColorId_of_Fire][State] < now - t_from) && !XBC_State.Is_LedOn(now))		TransitionTo_Wait();
			break;
			
		default:
			break;
	}
}

/******************************
******************************/
void ofApp::update(){
	/********************
	********************/
	now = ofGetElapsedTimeMillis();
	ofSoundUpdate();

	/********************
	********************/
	ReceiveOsc_from_XBC();
	
	StateChart();
	
	/********************
	********************/
	/* */
	LED_PARAM col = get_ColorofFile();
	
	// ofxVec4Slider のmemberへのaccessは dotでなく->である点に注意
	// これは単に、ofxVec4Sliderのoperatorでそう定義してあるため。
	// if(State == STATE__CHECK_LED) col = LED_PARAM(0.0, 0.0, 1.0, 0.0);
	if(State == STATE__CHECK_LED) col = LED_PARAM(Gui_Global->col_WhenTest->x, Gui_Global->col_WhenTest->y, Gui_Global->col_WhenTest->z, Gui_Global->col_WhenTest->w);
	
	for(int i = 0; i < NUM_LIGHTS_DYNAMIC; i++){
		Light_Dynamic[i].update(now, col, LED_PARAM(1.0, 1.0, 1.0, 1.0));
	}
	for(int i = 0; i < NUM_LIGHTS_CLIMAX; i++){
		Light_Climax[i].update(now, LED_PARAM(1.0, 0.6, 0.0, 0.0), LED_PARAM(1.0, 1.0, 1.0, 1.0));
	}
	for(int i = 0; i < NUM_LIGHTS_BACK; i++){
		Light_Back[i].update(now, LED_PARAM(1.0, 1.0, 1.0, 1.0), LED_PARAM(1.0, 1.0, 1.0, 1.0));
	}
	
	float VolOfSound = Cal_VolOfSound();
	for(int i = 0; i < NUM_LIGHTS_VOL; i++){
		Light_Volume[i].update_direct(VolOfSound, col);
	}
	
	/* */
	VolumeControl();
	
	ControlNebutaVoice();
	
	/********************
	********************/
	b_key_Enter = false;
	b_key_CheckLed = false;
	
	t_Last = now;
}

/******************************
******************************/
void ofApp::ControlNebutaVoice(){
	/********************
	********************/
	if( State < STATE__QUAKE_RISE) return;
	// if( (State != STATE__ON) && (State != STATE__ON_DIALOGUE) )	return;
	
	/********************
	********************/
	if(b_NebutaVoiceEnd) return;
	
	int t_NebutaVoice = now - t_NebutaVoice_Zero;
	
	int* t_NebutaVoiceStart;
	if(ColorId_of_Fire == COLOR_ID__CALM)	t_NebutaVoiceStart = t_NebutaVoiceStart_Calm;
	else									t_NebutaVoiceStart = t_NebutaVoiceStart_Evil;
	
	if(t_NebutaVoiceStart[NebutaVoiceId] <= t_NebutaVoice){
		if(!NebutaVoice[NebutaVoiceId].isPlaying()){
			NebutaVoice[NebutaVoiceId].play(ColorId_of_Fire == COLOR_ID__CALM);
			NebutaVoice[NebutaVoiceId].setVolume(1.0);
			NebutaVoice[NebutaVoiceId].setStartPositionMS();
		}
		
		NebutaVoiceId++;
		if(NUM_VOICES - 1 <= NebutaVoiceId) b_NebutaVoiceEnd = true;
	}
}

/******************************
******************************/
float ofApp::Cal_VolOfSound(){
	float alpha;
	if(Gui_Global->Smooth_dt <= 0)	alpha = 1.0;
	else							alpha = 1/Gui_Global->Smooth_dt * (now - t_Last);
	
	float * val = ofSoundGetSpectrum(1);
	float Vol = ofMap(val[0], 0, Gui_Global->fft_map_max, 0, 1.0, true);
	printf("%7.3f\r", val[0]);
	fflush(stdout);
	
	Vol = alpha * Vol + (1 - alpha) * LastVol_of_Sound;
	
	LastVol_of_Sound = Vol;
	
	return Vol;
}

/******************************
******************************/
void ofApp::volLightDynamic__set(double val){
	vol_Light_Intro.set(val);
	vol_Light_QuakeH.set(val);
	vol_Light_Face.set(val);
}

/******************************
******************************/
void ofApp::volLightDynamic__Down(double step, double limit){
	vol_Light_Intro.VolDown(step, limit);
	vol_Light_QuakeH.VolDown(step, limit);
	vol_Light_Face.VolDown(step, limit);
}

/******************************
******************************/
void ofApp::volLightDynamic__Up(double step, double limit){
	vol_Light_Intro.VolUp(step, limit);
	vol_Light_QuakeH.VolUp(step, limit);
	vol_Light_Face.VolUp(step, limit);
}

/******************************
******************************/
bool ofApp::volLightDynamic__IsZero(){
	if( (vol_Light_Intro.get() <= 0) && (vol_Light_QuakeH.get() <= 0) && (vol_Light_Face.get() <= 0) ){
		return true;
	}else{
		return false;
	}
}

/******************************
******************************/
void ofApp::VolumeControl(){
	enum{ NUM_MAXLEV_CLIMAX = 3, };
	
	const double volAmbient_max = 0.1; // 風音があまりうるさいと嫌なので.
	const double volFire_max = 0.6;
	
	// const double volClimax_max = 0.9;
	const double volClimax_max__Evil[NUM_MAXLEV_CLIMAX] = {0.9, 0.45, 1.0};
	const double volClimax_max__Calm[NUM_MAXLEV_CLIMAX] = {0.9, 0.9, 0.9};
	const double volClimax_Low_Calm = 0.05;
	const double volClimax_Low_Evil = 0.8;
	
	// const double volEnding_max = 0.7;
	const double volEnding_max = 0.4;
	
	const double step_slowest	= 1.0/d_eachState[ColorId_of_Fire][STATE__MAGMA] * (now - t_Last);
	const double step_slower	= 1.0/5000.0 * (now - t_Last);
	const double step_slow		= 1.0/3000.0 * (now - t_Last);
	const double step_norm		= 1.0/1000.0 * (now - t_Last);
	const double step_fast		= 1.0/500.0 * (now - t_Last);
	const double step_faster	= 1.0/200.0 * (now - t_Last);
	
	switch(State){
		case STATE__WAIT:
			volLightDynamic__Down(step_fast, 0.0);
			if(volLightDynamic__IsZero())	SetFrontPattern__On(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
			
			vol_Light_Climax.VolDown(step_fast);
			vol_Light_Back.VolUp(step_slow, Gui_Global->volLight_Back_max);
			vol_Light_Vol.VolDown(step_fast);
			
			sound_VolUp_AutoStart(sound_Ambient, step_slow, volAmbient_max);
			sound_VolDown_AutoStop(sound_Mysterious, step_slow);
			sound_VolDown_AutoStop(sound_Quake, step_slow);
			sound_VolDown_AutoStop(sound_Magma, step_slow);
			sound_VolDown_AutoStop(sound_Dooon, step_slow);
			sound_VolDown_AutoStop(sound_Fire, step_slow);
			sound_Climax.VolDown(step_slow);
			sound_Ending.VolDown(step_slow);
			
			vol_mov_Calm.VolDown(step_slow);
			vol_mov_Evil.VolDown(step_slow);
			break;
			
		case STATE__CHECK_LED:
			sound_VolDown_AutoStop(sound_Ambient, step_slow);
			sound_VolDown_AutoStop(sound_Mysterious, step_slow);
			sound_VolDown_AutoStop(sound_Quake, step_slow);
			sound_VolDown_AutoStop(sound_Magma, step_slow);
			sound_VolDown_AutoStop(sound_Dooon, step_slow);
			sound_VolDown_AutoStop(sound_Fire, step_slow);
			sound_Climax.VolDown(step_slow);
			sound_Ending.VolDown(step_slow);
			
			vol_mov_Calm.VolDown(step_slow);
			vol_mov_Evil.VolDown(step_slow);
			break;
			
		case STATE__MANUAL_ON:
			sound_VolDown_AutoStop(sound_Ambient, step_slow);
			sound_VolDown_AutoStop(sound_Mysterious, step_slow);
			sound_VolDown_AutoStop(sound_Quake, step_slow);
			sound_VolDown_AutoStop(sound_Magma, step_slow);
			sound_VolDown_AutoStop(sound_Dooon, step_slow);
			sound_VolDown_AutoStop(sound_Fire, step_slow);
			sound_Climax.VolDown(step_slow);
			sound_Ending.VolDown(step_slow);
			
			vol_mov_Calm.set(1.0);
			vol_mov_Evil.VolDown(step_slow);
			break;
			
		case STATE__INTRO_RISE:
			vol_Light_Intro.VolUp(step_fast, 1.0);
			vol_Light_QuakeH.VolDown(step_fast, 0.0);
			vol_Light_Face.VolDown(step_fast, 0.0);
			
			vol_Light_Climax.VolDown(step_fast);
			vol_Light_Back.VolUp(step_slow, Gui_Global->volLight_Back_max);
			vol_Light_Vol.VolDown(step_fast);
			
			sound_VolDown_AutoStop(sound_Ambient, step_slow);
			sound_VolUp_AutoStart(sound_Mysterious, step_slower);
			sound_VolDown_AutoStop(sound_Quake, step_slow);
			sound_VolDown_AutoStop(sound_Magma, step_slow);
			sound_VolDown_AutoStop(sound_Dooon, step_slow);
			sound_VolDown_AutoStop(sound_Fire, step_slow);
			sound_Climax.VolDown(step_slow);
			sound_Ending.VolDown(step_slow);
			
			if(ColorId_of_Fire == COLOR_ID__CALM)		{ vol_mov_Calm.VolUp(step_fast, 0.5); vol_mov_Evil.VolDown(step_fast); }
			else if(ColorId_of_Fire == COLOR_ID__EVIL)	{ vol_mov_Calm.VolDown(step_fast); vol_mov_Evil.VolUp(step_fast, 0.5); }
			else if(ColorId_of_Fire == COLOR_ID__BLACK)	{ vol_mov_Calm.VolDown(step_fast); vol_mov_Evil.VolDown(step_fast); } // ないはず.
			break;
			
		case STATE__INTRO_FALL:
			volLightDynamic__Down(step_fast);
			vol_Light_Climax.VolDown(step_fast);
			vol_Light_Back.VolDown(step_fast);
			vol_Light_Vol.VolDown(step_fast);

			sound_VolDown_AutoStop(sound_Ambient, step_slow);
			sound_VolUp_AutoStart(sound_Mysterious, step_slow);
			sound_VolUp_AutoStart(sound_Quake, step_slow, 0.5);
			sound_VolDown_AutoStop(sound_Magma, step_slow);
			sound_VolDown_AutoStop(sound_Dooon, step_slow);
			sound_VolDown_AutoStop(sound_Fire, step_slow);
			sound_Climax.VolDown(step_slow);
			sound_Ending.VolDown(step_slow);
			
			vol_mov_Calm.VolDown(step_fast);
			vol_mov_Evil.VolDown(step_fast);
			break;
			
		case STATE__QUAKE_RISE:
		{
			const double step_slow_Quake = 1.0/d_eachState[ColorId_of_Fire][STATE__QUAKE_RISE] * (now - t_Last);
			
			vol_Light_Intro.VolUp(step_slow_Quake, 1.0);
			vol_Light_QuakeH.VolUp(step_fast, 1.0);
			vol_Light_Face.VolDown(step_fast, 0.0);
			
			vol_Light_Climax.VolDown(step_fast);
			vol_Light_Back.VolDown(step_fast);
			vol_Light_Vol.VolDown(step_fast);

			sound_VolDown_AutoStop(sound_Ambient, step_slow);
			sound_VolUp_AutoStart(sound_Mysterious, step_slow);
			sound_VolUp_AutoStart(sound_Quake, step_slow, 0.5);
			sound_VolDown_AutoStop(sound_Magma, step_slow);
			sound_VolDown_AutoStop(sound_Dooon, step_slow);
			sound_VolDown_AutoStop(sound_Fire, step_slow);
			sound_Climax.VolDown(step_slow);
			sound_Ending.VolDown(step_slow);
			
			if(ColorId_of_Fire == COLOR_ID__CALM)		{ vol_mov_Calm.VolUp(step_fast); vol_mov_Evil.VolDown(step_fast); }
			else if(ColorId_of_Fire == COLOR_ID__EVIL)	{ vol_mov_Calm.VolDown(step_fast); vol_mov_Evil.VolUp(step_fast); }
			else if(ColorId_of_Fire == COLOR_ID__BLACK)	{ vol_mov_Calm.VolDown(step_fast); vol_mov_Evil.VolDown(step_fast); } // ないはず.
		}
			break;
			
		case STATE__QUAKE_FALL:
			volLightDynamic__Down(step_fast);
			vol_Light_Climax.VolDown(step_fast);
			vol_Light_Back.VolDown(step_fast);
			vol_Light_Vol.VolDown(step_fast);

			sound_VolDown_AutoStop(sound_Ambient, step_slow);
			sound_VolDown_AutoStop(sound_Mysterious, step_slow);
			sound_VolUp_AutoStart(sound_Quake, step_slow);
			sound_VolUp_AutoStart(sound_Magma, step_slow);
			sound_VolDown_AutoStop(sound_Dooon, step_slow);
			sound_VolDown_AutoStop(sound_Fire, step_slow);
			sound_Climax.VolDown(step_slow);
			sound_Ending.VolDown(step_slow);
			
			vol_mov_Calm.VolDown(step_fast);
			vol_mov_Evil.VolDown(step_fast);
			break;
			
		case STATE__MAGMA:
			vol_Light_Climax.VolDown(step_fast);
			vol_Light_Back.VolDown(step_fast);
			vol_Light_Vol.VolDown(step_fast);
			
			sound_VolDown_AutoStop(sound_Ambient, step_slow);
			sound_VolDown_AutoStop(sound_Mysterious, step_slow);
			sound_VolUp_AutoStart(sound_Quake, step_slow);
			sound_VolUp_AutoStart(sound_Magma, step_slow);
			sound_VolDown_AutoStop(sound_Dooon, step_slow);
			sound_VolDown_AutoStop(sound_Fire, step_slow);
			
			if(ColorId_of_Fire == COLOR_ID__CALM)		sound_Climax.VolUp(step_slowest, volClimax_max__Calm[0]);
			else if(ColorId_of_Fire == COLOR_ID__EVIL)	sound_Climax.VolUp(step_slowest, volClimax_max__Evil[0]);
			else if(ColorId_of_Fire == COLOR_ID__BLACK)	sound_Climax.VolUp(step_slowest, volClimax_max__Calm[0]); // ないはず.
			
			sound_Ending.VolDown(step_slow);
			
			{
				const double T = 4000;
				vol_mov_Calm.set(my_SinWave(T, 0, 0, 1));
				vol_mov_Evil.set(my_SinWave(T, 135, 0, 1));
			}
			break;
			
		case STATE__DARK:
		case STATE__FLYING:
			volLightDynamic__Down(step_fast);
			
			// vol_Light_Back.VolUp(step_fast, Gui_Global->volLight_Back_max);
			vol_Light_Back.VolUp(step_fast);
			vol_Light_Vol.VolDown(step_fast);
			
			sound_VolDown_AutoStop(sound_Ambient, step_fast);
			sound_VolDown_AutoStop(sound_Mysterious, step_fast);
			sound_VolDown_AutoStop(sound_Quake, step_fast);
			sound_VolDown_AutoStop(sound_Magma, step_fast);
			sound_VolDown_AutoStop(sound_Dooon, step_fast);
			sound_VolDown_AutoStop(sound_Fire, step_fast);
			
			if(ColorId_of_Fire == COLOR_ID__CALM)		sound_Climax.VolUp(step_slowest, volClimax_max__Calm[0]);
			else if(ColorId_of_Fire == COLOR_ID__EVIL)	sound_Climax.VolUp(step_slowest, volClimax_max__Evil[0]);
			else if(ColorId_of_Fire == COLOR_ID__BLACK)	sound_Climax.VolUp(step_slowest, volClimax_max__Calm[0]); // ないはず.
			
			sound_Ending.VolDown(step_slow);
			
			vol_mov_Calm.VolDown(step_fast);
			vol_mov_Evil.VolDown(step_fast);
			break;
			
		case STATE__ON:
			vol_Light_Back.VolDown(step_fast);
			
			sound_VolDown_AutoStop(sound_Ambient, step_fast);
			sound_VolDown_AutoStop(sound_Mysterious, step_fast);
			sound_VolDown_AutoStop(sound_Quake, step_fast);
			sound_VolDown_AutoStop(sound_Magma, step_fast);
			sound_VolUp_AutoStart(sound_Fire, step_slow, volFire_max);
			
			{
				int t_NebutaVoice = now - t_NebutaVoice_Zero;

				if(ColorId_of_Fire == COLOR_ID__CALM){
					sound_Climax.VolUp(step_slow, volClimax_max__Calm[2]);
				}else if(ColorId_of_Fire == COLOR_ID__EVIL){
					if(t_NebutaVoice < 32500)	sound_Climax.VolUp(step_slow, volClimax_max__Evil[1]);
					else						sound_Climax.VolUp(step_slow, volClimax_max__Evil[2]);
				}else if(ColorId_of_Fire == COLOR_ID__BLACK){
					sound_Climax.VolUp(step_slow, volClimax_max__Calm[2]); // ないはず
				}
			}
			
			sound_Ending.VolDown(step_slow);
			
			if(ColorId_of_Fire == COLOR_ID__CALM)		{ vol_mov_Calm.set(1.0); vol_mov_Evil.VolDown(step_fast); }
			else if(ColorId_of_Fire == COLOR_ID__EVIL)	{ vol_mov_Calm.VolDown(step_fast); vol_mov_Evil.set(1.0); }
			else if(ColorId_of_Fire == COLOR_ID__BLACK)	{ vol_mov_Calm.VolDown(step_fast); vol_mov_Evil.VolDown(step_fast); } // ないはず.
			break;
			
		case STATE__ON_DIALOGUE:
		{
			const double step_slow_Dialogue = 1.0/d_eachState[ColorId_of_Fire][STATE__ON_DIALOGUE] * 3/4 * (now - t_Last);
			vol_Light_Climax.VolDown(step_slow_Dialogue, 0.4);
			
			vol_Light_Back.VolDown(step_fast);
			
			sound_VolDown_AutoStop(sound_Ambient, step_fast);
			sound_VolDown_AutoStop(sound_Mysterious, step_fast);
			sound_VolDown_AutoStop(sound_Quake, step_fast);
			sound_VolDown_AutoStop(sound_Magma, step_fast);
			sound_VolUp_AutoStart(sound_Fire, step_slow, volFire_max);
			if(ColorId_of_Fire == COLOR_ID__CALM)	sound_Climax.VolDown(step_slow, volClimax_Low_Calm);
			else									sound_Climax.VolDown(step_slow, volClimax_Low_Evil);
			sound_Ending.VolDown(step_slow);
			
			if(ColorId_of_Fire == COLOR_ID__CALM)		{ vol_mov_Calm.set(1.0); vol_mov_Evil.VolDown(step_fast); }
			else if(ColorId_of_Fire == COLOR_ID__EVIL)	{ vol_mov_Calm.VolDown(step_fast); vol_mov_Evil.set(1.0); }
			else if(ColorId_of_Fire == COLOR_ID__BLACK)	{ vol_mov_Calm.VolDown(step_fast); vol_mov_Evil.VolDown(step_fast); } // ないはず.
		}
			break;
			
		case STATE__FADE_TO_END:
		case STATE__NOSOUND:
		{
			const double step_climax = 1.0/d_eachState[ColorId_of_Fire][STATE__FADE_TO_END] * (now - t_Last);
			
			vol_Light_Climax.VolDown(step_fast);
			
			sound_VolDown_AutoStop(sound_Ambient, step_fast);
			sound_VolDown_AutoStop(sound_Mysterious, step_fast);
			sound_VolDown_AutoStop(sound_Quake, step_fast);
			sound_VolDown_AutoStop(sound_Magma, step_fast);
			sound_VolUp_AutoStart(sound_Fire, step_slow, volFire_max);
			sound_Climax.VolDown(step_climax);
			sound_Ending.VolDown(step_slow);
			
			vol_mov_Calm.VolDown(step_climax);
			vol_mov_Evil.VolDown(step_climax);
		}
			break;
			
		case STATE__ENDING:
		{
			const double step_Ending = 1.0/10000 * (now - t_Last);
			
			volLightDynamic__Down(step_fast);
			// vol_Light_Climax.VolUp(step_norm);
			vol_Light_Back.VolDown(step_fast);
			
			sound_VolDown_AutoStop(sound_Ambient, step_fast);
			sound_VolDown_AutoStop(sound_Mysterious, step_fast);
			sound_VolDown_AutoStop(sound_Quake, step_fast);
			sound_VolDown_AutoStop(sound_Magma, step_fast);
			sound_VolDown_AutoStop(sound_Dooon, step_fast);
			sound_VolUp_AutoStart(sound_Fire, step_slow, volFire_max);
			sound_Climax.VolDown(step_fast);
			sound_Ending.VolUp(step_Ending, volEnding_max);
			
			if(ColorId_of_Fire == COLOR_ID__CALM)		{ vol_mov_Calm.VolUp(step_fast); vol_mov_Evil.VolDown(step_fast); }
			else if(ColorId_of_Fire == COLOR_ID__EVIL)	{ vol_mov_Calm.VolDown(step_fast); vol_mov_Evil.VolUp(step_fast); }
			else if(ColorId_of_Fire == COLOR_ID__BLACK)	{ vol_mov_Calm.VolDown(step_fast); vol_mov_Evil.VolDown(step_fast); } // ないはず.
		}
			break;
			
		case STATE__FADEOUT:
			volLightDynamic__Down(step_slow);
			vol_Light_Climax.VolDown(step_slow);
			vol_Light_Back.VolUp(step_slow, Gui_Global->volLight_Back_max);
			vol_Light_Vol.VolDown(step_slow);
			
			sound_VolUp_AutoStart(sound_Ambient, step_slow, volAmbient_max);
			sound_VolDown_AutoStop(sound_Mysterious, step_slow);
			sound_VolDown_AutoStop(sound_Quake, step_slow);
			sound_VolDown_AutoStop(sound_Magma, step_slow);
			sound_VolDown_AutoStop(sound_Dooon, step_slow);
			sound_VolDown_AutoStop(sound_Fire, step_slow);
			sound_Climax.VolDown(step_slow);
			sound_Ending.VolDown(step_slow);
			
			vol_mov_Calm.VolDown(step_slow);
			vol_mov_Evil.VolDown(step_slow);
			break;
			
		default:
			break;
	}
}

/******************************
******************************/
double ofApp::my_SinWave(double T, double theta, double _min, double _max){
	double dt = T/360.0 * theta;
	return ofMap(sin(2 * PI * (now - dt) / T), -1, 1, _min, _max, true);
}

/******************************
******************************/
void ofApp::draw(){
	/********************
	********************/
	ofBackground(30);
	
	/********************
	********************/
	/*
	ofEnableAlphaBlending();
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	*/
	ofDisableAlphaBlending();
	ofEnableSmoothing();
	
	/********************
	********************/
	draw_info();
	
	sendDmx();
	SendOSC();
	
	// printf("%5.3f\r", sound_Climax.getPosition());
	// printf("%d\r", sound_Climax.getPositionMS());
	/*
	printf("%5.2f\r", ofGetFrameRate());
	fflush(stdout);
	*/
	
	/********************
	********************/
	Gui_Global->gui.draw();
}

/******************************
******************************/
void ofApp::update_ColorOfFire(){
	if(XBC_State.Is_Calm(now))		ColorId_of_Fire = COLOR_ID__CALM; // in case of both Calm and Evil, system choose "Calm".
	else if(XBC_State.Is_Evil(now))	ColorId_of_Fire = COLOR_ID__EVIL;
	// else							ColorId_of_Fire = COLOR_ID__BLACK; // no change.
}

/******************************
******************************/
LED_PARAM ofApp::get_ColorofFile(){
	switch(ColorId_of_Fire){
		case COLOR_ID__CALM:
			return LED_PARAM(0, 0, 1.0, 0);
		case COLOR_ID__EVIL:
			return LED_PARAM(1.0, 0, 0, 0);
		case COLOR_ID__BLACK:
			return LED_PARAM(0, 0, 0, 0);
	}
}

/******************************
******************************/
void ofApp::SendOSC(){
	/* */
	ofxOscMessage m;
	m.setAddress("/XBE");
	
	m.addFloatArg((float)vol_mov_Calm.get());
	m.addFloatArg((float)vol_mov_Evil.get());
	
	/* */
	Osc_Video_Nebuta.OscSend.sendMessage(m);
}

/******************************
******************************/
void ofApp::SendOSC_Clear(){
	/* */
	ofxOscMessage m;
	m.setAddress("/XBE");
	
	m.addFloatArg(0.0);
	m.addFloatArg(0.0);
	
	/* */
	Osc_Video_Nebuta.OscSend.sendMessage(m);
}

/******************************
******************************/
void ofApp::draw_info()
{
	/********************
	********************/
	ofSetColor(255);
	
	const int TextHeight = font[FONT_M].stringHeight("(A,yq]") * 1;
	char buf[BUF_SIZE_S];
	
	switch(State){
		case STATE__WAIT:
			if(b_skip)	sprintf(buf, "WAIT to skip : %5d", (now - t_from)/1000);
			else		sprintf(buf, "WAIT");
			break;
		case STATE__CHECK_LED:
			sprintf(buf, "CHECK\nLedId = %3d", LedId_Test);
			break;
		case STATE__MANUAL_ON:
			sprintf(buf, "MANUAL_ON");
			break;
		case STATE__INTRO_RISE:
			sprintf(buf, "INTRO_RISE");
			break;
		case STATE__INTRO_FALL:
			sprintf(buf, "INTRO_FALL");
			break;
		case STATE__QUAKE_RISE:
			sprintf(buf, "QUAKE_RISE");
			break;
		case STATE__QUAKE_FALL:
			sprintf(buf, "QUAKE_FALL");
			break;
		case STATE__MAGMA:
			sprintf(buf, "MAGMA");
			break;
		case STATE__DARK:
			sprintf(buf, "DARK");
			break;
		case STATE__FLYING:
			sprintf(buf, "FLYING");
			break;
		case STATE__ON:
			sprintf(buf, "ON");
			break;
		case STATE__ON_DIALOGUE:
			sprintf(buf, "ON_DIALOGUE");
			break;
		case STATE__FADE_TO_END:
			sprintf(buf, "FADE_TO_END");
			break;
		case STATE__NOSOUND:
			sprintf(buf, "NOSOUND");
			break;
		case STATE__ENDING:
			sprintf(buf, "ENDING : push Enter x2 : %d", c_toFadeOut);
			break;
		case STATE__FADEOUT:
			if(now - t_from < d_eachState[ColorId_of_Fire][State])	sprintf(buf, "FADEOUT:just a moment please.");
			else									sprintf(buf, "FADEOUT:move the Led away.");
			break;
		default:
			break;
	}
	
	font[FONT_M].drawString(buf, ofGetWidth()/2 - font[FONT_M].stringWidth(buf)/2, ofGetHeight()/2);
	
	/********************
	********************/
	{
		int _width = font[FONT_S].stringWidth("xxxxxxxxxxxxxxxxxxxxx");
		sprintf(buf, "%8.1f fps", ofGetFrameRate());
		font[FONT_S].drawString(buf, ofGetWidth() - _width, TextHeight);
		
		sprintf(buf, "%8.3f volume", LastVol_of_Sound);
		font[FONT_S].drawString(buf, ofGetWidth() - _width, TextHeight * 2);
		
		sprintf(buf, "%8.3f climax vol", sound_Climax.getVolume());
		font[FONT_S].drawString(buf, ofGetWidth() - _width, TextHeight * 3);
	}
	
	/********************
	********************/
	{
		int _width = font[FONT_M].stringWidth("XXXXXXX");
		
		int dt = now - t_from;
		int TimeLeft = (d_eachState[ColorId_of_Fire][State] - dt)/1000;
		
		if(TimeLeft <= 0)	sprintf(buf, "---");
		else				sprintf(buf, "%5d", TimeLeft);
		
		font[FONT_M].drawString(buf, ofGetWidth() - _width, ofGetHeight() - TextHeight);
	}
	
	/********************
	********************/
	ofPushMatrix();
	ofTranslate(0, ofGetHeight());
	ofScale(1, -1, 1);
		const int size = 20;
		if(XBC_State.Is_Calm(now)){
			ofSetColor(0, 0, 255);
			ofDrawRectangle(size, size, size, size);
		}
		if(XBC_State.Is_Evil(now)){
			ofSetColor(255, 0, 0);
			ofDrawRectangle(3 * size, size, size, size);
		}
		
		if(ColorId_of_Fire == COLOR_ID__BLACK)		ofSetColor(0, 0, 0);
		else if(ColorId_of_Fire == COLOR_ID__CALM)	ofSetColor(0, 0, 255);
		else if(ColorId_of_Fire == COLOR_ID__EVIL)	ofSetColor(255, 0, 0);
		ofDrawRectangle(size, 3 * size, size, size);
		
		if(XBC_State.Is_Motion(now)){
			ofSetColor(180);
			ofDrawRectangle(size, 5 * size, size, size);
		}
	ofPopMatrix();
}

/******************************
******************************/
void ofApp::sendDmx()
{
	/********************
	********************/
	if(b_DmxShutter_Open)	ode[DmxShutter.ODE_id].universe[DmxShutter.AddressFrom] = int(Gui_Global->DmxShutter_open);
	else					ode[DmxShutter.ODE_id].universe[DmxShutter.AddressFrom] = int(Gui_Global->DmxShutter_close);
	
	/********************
	Dynamic Light : AiFarm
	********************/
	for(int LightType = 0; LightType < 3; LightType++){
		/* */
		const int* id_Array;
		VOLUME* vol_Ligt;
		
		if(LightType == 0)		{ id_Array = id_Intro; vol_Ligt = &vol_Light_Intro; }
		else if(LightType == 1)	{ id_Array = id_QuakeH; vol_Ligt = &vol_Light_QuakeH; }
		else if(LightType == 2)	{ id_Array = id_Face; vol_Ligt = &vol_Light_Face; }
		
		/* */
		for(int i = 0; id_Array[i] != -1; i++){
			int id = id_Array[i];
			
			switch(Light_Dynamic[id].LedDeviceType){
				case LED_DEVICE_TYPE_FIXED:
					ode[ Light_Dynamic[id].DmxStorage.ODE_id ].universe[ Light_Dynamic[id].DmxStorage.AddressFrom + 0 ] = int(255 * Gui_Global->Led_dimmer); // dimmer
					ode[ Light_Dynamic[id].DmxStorage.ODE_id ].universe[ Light_Dynamic[id].DmxStorage.AddressFrom + 1 ] = ofMap(Light_Dynamic[id].LedParam_Out.r * vol_Ligt->get(), 0, 1.0, 0, 255, true);
					ode[ Light_Dynamic[id].DmxStorage.ODE_id ].universe[ Light_Dynamic[id].DmxStorage.AddressFrom + 2 ] = ofMap(Light_Dynamic[id].LedParam_Out.g * vol_Ligt->get(), 0, 1.0, 0, 255, true);
					ode[ Light_Dynamic[id].DmxStorage.ODE_id ].universe[ Light_Dynamic[id].DmxStorage.AddressFrom + 3 ] = ofMap(Light_Dynamic[id].LedParam_Out.b * vol_Ligt->get(), 0, 1.0, 0, 255, true);
					ode[ Light_Dynamic[id].DmxStorage.ODE_id ].universe[ Light_Dynamic[id].DmxStorage.AddressFrom + 4 ] = ofMap(Light_Dynamic[id].LedParam_Out.w * vol_Ligt->get(), 0, 1.0, 0, 255, true);
					ode[ Light_Dynamic[id].DmxStorage.ODE_id ].universe[ Light_Dynamic[id].DmxStorage.AddressFrom + 5 ] = 1; // Strobe = open.
					break;
					
				case LED_DEVICE_RGB:
					ode[ Light_Dynamic[id].DmxStorage.ODE_id ].universe[ Light_Dynamic[id].DmxStorage.AddressFrom + 0 ] = ofMap(Light_Dynamic[id].LedParam_Out.r * vol_Ligt->get() * Gui_Global->Led_dimmer, 0, 1.0, 0, 255, true);
					ode[ Light_Dynamic[id].DmxStorage.ODE_id ].universe[ Light_Dynamic[id].DmxStorage.AddressFrom + 1 ] = ofMap(Light_Dynamic[id].LedParam_Out.g * vol_Ligt->get() * Gui_Global->Led_dimmer, 0, 1.0, 0, 255, true);
					ode[ Light_Dynamic[id].DmxStorage.ODE_id ].universe[ Light_Dynamic[id].DmxStorage.AddressFrom + 2 ] = ofMap(Light_Dynamic[id].LedParam_Out.b * vol_Ligt->get() * Gui_Global->Led_dimmer, 0, 1.0, 0, 255, true);
					break;
					
				case LED_DEVICE_1CH:
					ode[ Light_Dynamic[id].DmxStorage.ODE_id ].universe[ Light_Dynamic[id].DmxStorage.AddressFrom + 0 ] = ofMap(Light_Dynamic[id].LedParam_Out.get_max() * vol_Ligt->get() * Gui_Global->Led_dimmer, 0, 1.0, 0, 255, true);
					break;
					
				default:
					break;
			}			
		}
	}
	
	/********************
	********************/
	for(int LightType = 0; LightType < 3; LightType++){
		/* */
		LED_LIGHT* LedLight;
		int NUM_LEDS;
		VOLUME* vol_Ligt;
		
		if(LightType == 0)		{ LedLight = Light_Climax; NUM_LEDS = NUM_LIGHTS_CLIMAX; vol_Ligt = &vol_Light_Climax; }
		else if(LightType == 1)	{ LedLight = Light_Back; NUM_LEDS = NUM_LIGHTS_BACK; vol_Ligt = &vol_Light_Back; }
		else if(LightType == 2)	{ LedLight = Light_Volume; NUM_LEDS = NUM_LIGHTS_VOL; vol_Ligt = &vol_Light_Vol; }
		
		/* */
		for(int i = 0; i < NUM_LEDS; i++){
			switch(LedLight[i].LedDeviceType){
				case LED_DEVICE_TYPE_FIXED:
					// ode[ LedLight[i].DmxStorage.ODE_id ].universe[ LedLight[i].DmxStorage.AddressFrom + 0 ] = 255; // dimmer
					ode[ LedLight[i].DmxStorage.ODE_id ].universe[ LedLight[i].DmxStorage.AddressFrom + 0 ] = int(255 * Gui_Global->Led_dimmer); // dimmer
					ode[ LedLight[i].DmxStorage.ODE_id ].universe[ LedLight[i].DmxStorage.AddressFrom + 1 ] = ofMap(LedLight[i].LedParam_Out.r * vol_Ligt->get(), 0, 1.0, 0, 255, true);
					ode[ LedLight[i].DmxStorage.ODE_id ].universe[ LedLight[i].DmxStorage.AddressFrom + 2 ] = ofMap(LedLight[i].LedParam_Out.g * vol_Ligt->get(), 0, 1.0, 0, 255, true);
					ode[ LedLight[i].DmxStorage.ODE_id ].universe[ LedLight[i].DmxStorage.AddressFrom + 3 ] = ofMap(LedLight[i].LedParam_Out.b * vol_Ligt->get(), 0, 1.0, 0, 255, true);
					ode[ LedLight[i].DmxStorage.ODE_id ].universe[ LedLight[i].DmxStorage.AddressFrom + 4 ] = ofMap(LedLight[i].LedParam_Out.w * vol_Ligt->get(), 0, 1.0, 0, 255, true);
					ode[ LedLight[i].DmxStorage.ODE_id ].universe[ LedLight[i].DmxStorage.AddressFrom + 5 ] = 1; // Strobe = open.
					break;
					
				case LED_DEVICE_RGB:
					ode[ LedLight[i].DmxStorage.ODE_id ].universe[ LedLight[i].DmxStorage.AddressFrom + 0 ] = ofMap(LedLight[i].LedParam_Out.r * vol_Ligt->get() * Gui_Global->Led_dimmer, 0, 1.0, 0, 255, true);
					ode[ LedLight[i].DmxStorage.ODE_id ].universe[ LedLight[i].DmxStorage.AddressFrom + 1 ] = ofMap(LedLight[i].LedParam_Out.g * vol_Ligt->get() * Gui_Global->Led_dimmer, 0, 1.0, 0, 255, true);
					ode[ LedLight[i].DmxStorage.ODE_id ].universe[ LedLight[i].DmxStorage.AddressFrom + 2 ] = ofMap(LedLight[i].LedParam_Out.b * vol_Ligt->get() * Gui_Global->Led_dimmer, 0, 1.0, 0, 255, true);
					break;
					
				case LED_DEVICE_1CH:
					ode[ LedLight[i].DmxStorage.ODE_id ].universe[ LedLight[i].DmxStorage.AddressFrom + 0 ] = ofMap(LedLight[i].LedParam_Out.get_max() * vol_Ligt->get() * Gui_Global->Led_dimmer, 0, 1.0, 0, 255, true);
					break;
					
				default:
					break;
			}
		}
	}
	
	/********************
	********************/
	for(int i = 0; i < NUM_ODES; i++){
		ode[i].SendDmx();
	}
}

/******************************
******************************/
void ofApp::keyPressed(int key){

	switch(key){
		case OF_KEY_DOWN:
			LedId_Test--;
			if(LedId_Test < -1) LedId_Test = -1;
			SetFrontPattern__CheckLed(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
			break;
			
		case OF_KEY_UP:
			LedId_Test++;
			if(NUM_LIGHTS_DYNAMIC <= LedId_Test) LedId_Test = NUM_LIGHTS_DYNAMIC;			
			SetFrontPattern__CheckLed(Light_Dynamic, NUM_LIGHTS_DYNAMIC);
			break;
			
		case OF_KEY_RETURN:
			b_key_Enter = true;
			break;
			
		case 't':
			b_key_CheckLed = true;
			break;
			
		case 'r':
			XBC_State.KeyToggle_Evil();
			break;
			
		case 'b':
			XBC_State.KeyToggle_Calm();
			break;
			
		case 'm':
			XBC_State.KeyToggle_Motion();
			break;
	}
}

/******************************
******************************/
void ofApp::keyReleased(int key){

}

/******************************
******************************/
void ofApp::mouseMoved(int x, int y ){

}

/******************************
******************************/
void ofApp::mouseDragged(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mousePressed(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mouseReleased(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mouseEntered(int x, int y){

}

/******************************
******************************/
void ofApp::mouseExited(int x, int y){

}

/******************************
******************************/
void ofApp::windowResized(int w, int h){

}

/******************************
******************************/
void ofApp::gotMessage(ofMessage msg){

}

/******************************
******************************/
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
