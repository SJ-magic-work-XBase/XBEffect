/************************************************************
************************************************************/
#include "ofApp.h"

/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp()
: Osc_XBC("127.0.0.1", 12346, 12345)
, Osc_Video_Nebuta("127.0.0.1", 12350, 12349)
, Osc_Video_Wall("127.0.0.1", 12348, 12347)
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
	
	SetLightPattern__On();
	
	LoadAndSet_sounds();
	
	/********************
	********************/
#ifdef ARTNET_THREADED
	artnet.enableThread(fps);	// main windowと同じにしておくのが安全かな.
#endif
}

/******************************
******************************/
void ofApp::LoadAndSet_sounds()
{
	setup_sound(sound_Mysterious, "_SelectedSound/mysterious-dungeons-ambiance_GkvZUHVO.wav", true, 0.0);
	setup_sound(sound_Quake, "_SelectedSound/lava-flowing_f1gkErVd.wav", true, 0.0);
	setup_sound(sound_Magma, "_SelectedSound/flowing-magma-foley_MJC0mBNu.wav", true, 0.0);
	setup_sound(sound_Thunder, "_SelectedSound/thunder-clap_z19asnEO.wav", false, 1.0);
	setup_sound(sound_Dooon, "_SelectedSound/explosion-blast-large-bass-rumble_fk5jCFVu.wav", false, 1.0);
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
		sound.setPosition(0);
		sound.play();
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
void ofApp::SetLightPattern__CheckLed()
{
	for(int i = 0; i < NUM_LEDS; i++){
		if(i == LedId_Test)	LedLight[i].LightPattern_Front.setup(now, 1.0);
		else				LedLight[i].LightPattern_Front.setup(now, 0.0);
	}
}

/******************************
******************************/
void ofApp::SetLightPattern__On()
{
	for(int i = 0; i < NUM_LEDS; i++){
		LedLight[i].LightPattern_Front.setup(now, 1.0);
	}
}

/******************************
******************************/
void ofApp::SetLightPattern__Perlin()
{
	for(int i = 0; i < NUM_LEDS; i++){
		// ofx_SET_LIGHTPATTERN::setup__Perlin(&LedLight[i].LightPattern_Front, now, 0, 1.0, 2000);
		ofx_SET_LIGHTPATTERN::setup__Perlin(&LedLight[i].LightPattern_Front, now, -0.5, 1.5, 1000);
	}
}

/******************************
******************************/
void ofApp::SetLightPattern__Strobe()
{
	for(int i = 0; i < NUM_LEDS; i++){
		setup__RandomStrobe(&LedLight[i].LightPattern_Front, now, 0.0, 1.0);
	}
}

/******************************
******************************/
void ofApp::setup__RandomStrobe(ofx_LIGHTPATTERN* LightPattern, int now_ms, double L0, double L1)
{
	/********************
	********************/
	int d_d_ms = 33; // 1 strobeの鋭さ
	int d_Transition_T_ms = d_eachState[STATE__QUAKE_RISE]/2; // 遷移時間
	
	/*
	どの時間を見てもX個のLedが光っている(実際はRange内でRandom selectなので、期待値)
		T = d_d_ms * NUM_LEDS / X;
	
	常に、全てのLedが光っている(期待値)
		T = d_d_ms * NUM_LEDS / NUM_LEDS;
		
	0.5個(疎)なら
		T = d_d_ms * NUM_LEDS / 0.5;
	*/
	ofx_LIGHTPATTERN::MIN_MAX_PAIR T_from( d_d_ms * NUM_LEDS/(max(NUM_LEDS/10, 1)), d_d_ms * NUM_LEDS/(max(NUM_LEDS/12, 1)) );
	ofx_LIGHTPATTERN::MIN_MAX_PAIR T_to( d_d_ms * NUM_LEDS/(max(NUM_LEDS/5, 1)), d_d_ms * NUM_LEDS/(max(NUM_LEDS/2, 1)) );
	
	/* */
	LightPattern->setup(now_ms, L0, L1, 0, 0, d_d_ms, T_from, T_to, d_Transition_T_ms);
}

/******************************
******************************/
void ofApp::SetLightPattern_Back__Off()
{
	for(int i = 0; i < NUM_LEDS; i++){
		LedLight[i].LightPattern_Back.setup(now, 0.0);
	}
}

/******************************
******************************/
void ofApp::SetLightPattern_Back__1Time_Flash()
{
	for(int i = 0; i < NUM_LEDS; i++){
		ofx_SET_LIGHTPATTERN::setup__1Time_Flash(&LedLight[i].LightPattern_Back, now, 0.0, 1.0, 600);
	}
}

/******************************
******************************/
void ofApp::DmxShutter_open()
{
}

/******************************
******************************/
void ofApp::DmxShutter_close()
{
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
	
	// SetLightPattern__On();
	SetLightPattern_Back__Off();
	
	DmxShutter_close();
	
	ColorId_of_Fire = COLOR_ID__BLACK;
}

/******************************
******************************/
void ofApp::TransitionTo_CheckLed(){
	State = STATE__CHECK_LED;
	t_from = now;
	
	SetLightPattern__CheckLed();
	SetLightPattern_Back__Off();
	LedId_Test = -1;
	
	vol_Ligt.set(1.0);
}

/******************************
******************************/
void ofApp::TransitionTo_ManualOn(){
	State = STATE__MANUAL_ON;
	t_from = now;
	SetLightPattern__On();
	SetLightPattern_Back__Off();
	DmxShutter_open();
	
	vol_Ligt.set(1.0);
}

/******************************
******************************/
void ofApp::TransitionTo_IntroRise(){
	State = STATE__INTRO_RISE;
	t_from = now;
	SetLightPattern_Back__Off();
	
	update_ColorOfFire();
}

/******************************
******************************/
void ofApp::TransitionTo_IntroFall(){
	State = STATE__INTRO_FALL;
	t_from = now;
	SetLightPattern_Back__Off();
}

/******************************
******************************/
void ofApp::TransitionTo_QuakeRise(){
	State = STATE__QUAKE_RISE;
	t_from = now;
	SetLightPattern__Perlin();
	SetLightPattern_Back__Off();
	
	update_ColorOfFire();
}

/******************************
******************************/
void ofApp::TransitionTo_QuakeFall(){
	State = STATE__QUAKE_FALL;
	t_from = now;
	SetLightPattern_Back__Off();
}

/******************************
******************************/
void ofApp::TransitionTo_Magma(){
	State = STATE__MAGMA;
	t_from = now;
	SetLightPattern__Strobe();
	SetLightPattern_Back__1Time_Flash();
	vol_Ligt.set(1.0);
	
	sound_Thunder.play();
	
	update_ColorOfFire();
}

/******************************
******************************/
void ofApp::TransitionTo_Dark(){
	State = STATE__DARK;
	t_from = now;
	SetLightPattern__On();
	SetLightPattern_Back__Off();
	vol_Ligt.set(0.0);
}

/******************************
******************************/
void ofApp::TransitionTo_On(){
	State = STATE__ON;
	t_from = now;
	SetLightPattern__On();
	SetLightPattern_Back__Off();
	vol_Ligt.set(1.0);
	
	DmxShutter_open();
	
	sound_Dooon.setVolume(1.0);
	sound_Dooon.play();
	
	update_ColorOfFire();
}

/******************************
******************************/
void ofApp::TransitionTo_FadeOut(){
	State = STATE__FADEOUT;
	t_from = now;
}

/******************************
******************************/
void ofApp::StateChart(){
	switch(State){
		case STATE__WAIT:
			if(b_key_CheckLed)					TransitionTo_CheckLed();
			else if(b_key_Enter)				TransitionTo_ManualOn();
			else if(XBC_State.Is_LedOn(now))	TransitionTo_IntroRise();
			break;
			
		case STATE__CHECK_LED:
			if(b_key_CheckLed){
				TransitionTo_Wait();
				SetLightPattern__On(); // ここは、明示的にsetする必要あり.
				vol_Ligt.set(0.0);
			}
			break;
			
		case STATE__MANUAL_ON:
			if(b_key_Enter){
				TransitionTo_Wait();
				SetLightPattern__On(); // ここは、明示的にsetする必要あり.
				vol_Ligt.set(0.0);
			}
			break;
			
		case STATE__INTRO_RISE:
			if(!XBC_State.Is_LedOn(now))				TransitionTo_Wait();
			else if(d_eachState[State] < now - t_from)	TransitionTo_IntroFall();
			break;
			
		case STATE__INTRO_FALL:
			if(!XBC_State.Is_LedOn(now))				TransitionTo_Wait();
			else if(d_eachState[State] < now - t_from)	TransitionTo_QuakeRise();
			break;
			
		case STATE__QUAKE_RISE:
			if(!XBC_State.Is_LedOn(now))				TransitionTo_Wait();
			else if(d_eachState[State] < now - t_from)	TransitionTo_QuakeFall();
			break;
			
		case STATE__QUAKE_FALL:
			if(!XBC_State.Is_LedOn(now))				TransitionTo_Wait();
			else if(d_eachState[State] < now - t_from)	TransitionTo_Magma();
			break;
			
		case STATE__MAGMA:
			if(d_eachState[State] < now - t_from)		TransitionTo_Dark();
			break;
			
		case STATE__DARK:
			if(d_eachState[State] < now - t_from)		TransitionTo_On();
			break;
			
		case STATE__ON:
			if(b_key_Enter)	TransitionTo_FadeOut();
			break;
			
		case STATE__FADEOUT:
			if((d_eachState[State] < now - t_from) && !XBC_State.Is_LedOn(now))		TransitionTo_Wait();
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
	
	for(int i = 0; i < NUM_LEDS; i++){
		LedLight[i].update(now, col, LED_PARAM(1.0, 1.0, 1.0, 1.0));
	}
	
	/* */
	VolumeControl();
	
	/********************
	********************/
	b_key_Enter = false;
	b_key_CheckLed = false;
	
	t_Last = now;
}

/******************************
	ofSoundPlayer sound_Mysterious;
	ofSoundPlayer sound_Quake;
	ofSoundPlayer sound_Magma;

void ofApp::sound_VolUp_AutoStart(ofSoundPlayer& sound, double step)
void ofApp::sound_VolDown_AutoStop(ofSoundPlayer& sound, double step)

******************************/
void ofApp::VolumeControl(){
	const double step_slower	= 1.0/3000.0 * (now - t_Last);
	const double step_slow		= 1.0/2000.0 * (now - t_Last);
	const double step_norm		= 1.0/1000.0 * (now - t_Last);
	const double step_fast		= 1.0/500.0 * (now - t_Last);
	const double step_faster	= 1.0/200.0 * (now - t_Last);
	
	switch(State){
		case STATE__WAIT:
			vol_Ligt.VolDown(step_fast, 0.0);
			if(vol_Ligt.get() <= 0)	SetLightPattern__On();
			
			sound_VolDown_AutoStop(sound_Mysterious, step_slow);
			sound_VolDown_AutoStop(sound_Quake, step_norm);
			sound_VolDown_AutoStop(sound_Magma, step_norm);
			sound_VolDown_AutoStop(sound_Dooon, step_norm);
			break;
			
		case STATE__CHECK_LED:
			sound_VolDown_AutoStop(sound_Mysterious, step_slow);
			sound_VolDown_AutoStop(sound_Quake, step_slow);
			sound_VolDown_AutoStop(sound_Magma, step_slow);
			sound_VolDown_AutoStop(sound_Dooon, step_slow);
			break;
			
		case STATE__MANUAL_ON:
			sound_VolDown_AutoStop(sound_Mysterious, step_slow);
			sound_VolDown_AutoStop(sound_Quake, step_slow);
			sound_VolDown_AutoStop(sound_Magma, step_slow);
			sound_VolDown_AutoStop(sound_Dooon, step_slow);
			break;
			
		case STATE__INTRO_RISE:
			vol_Ligt.VolUp(step_fast, 0.5);

			sound_VolUp_AutoStart(sound_Mysterious, step_slow);
			sound_VolDown_AutoStop(sound_Quake, step_slow);
			sound_VolDown_AutoStop(sound_Magma, step_slow);
			sound_VolDown_AutoStop(sound_Dooon, step_slow);
			break;
			
		case STATE__INTRO_FALL:
			vol_Ligt.VolDown(step_fast, 0.0);

			sound_VolUp_AutoStart(sound_Mysterious, step_slow);
			sound_VolUp_AutoStart(sound_Quake, step_slow, 0.5);
			sound_VolDown_AutoStop(sound_Magma, step_slow);
			sound_VolDown_AutoStop(sound_Dooon, step_slow);
			break;
			
		case STATE__QUAKE_RISE:
			vol_Ligt.VolUp(step_fast, 1.0);

			sound_VolUp_AutoStart(sound_Mysterious, step_slow);
			sound_VolUp_AutoStart(sound_Quake, step_slow, 0.5);
			sound_VolDown_AutoStop(sound_Magma, step_slow);
			sound_VolDown_AutoStop(sound_Dooon, step_slow);
			break;
			
		case STATE__QUAKE_FALL:
			vol_Ligt.VolDown(step_fast, 0.0);

			sound_VolDown_AutoStop(sound_Mysterious, step_slow);
			sound_VolUp_AutoStart(sound_Quake, step_slow);
			sound_VolUp_AutoStart(sound_Magma, step_slow);
			sound_VolDown_AutoStop(sound_Dooon, step_slow);
			break;
			
		case STATE__MAGMA:
			sound_VolDown_AutoStop(sound_Mysterious, step_slow);
			sound_VolUp_AutoStart(sound_Quake, step_slow);
			sound_VolUp_AutoStart(sound_Magma, step_slow);
			sound_VolDown_AutoStop(sound_Dooon, step_slow);
			break;
			
		case STATE__DARK:
			sound_VolDown_AutoStop(sound_Mysterious, step_fast);
			sound_VolDown_AutoStop(sound_Quake, step_fast);
			sound_VolDown_AutoStop(sound_Magma, step_fast);
			sound_VolDown_AutoStop(sound_Dooon, step_fast);
			break;
			
		case STATE__ON:
			sound_VolDown_AutoStop(sound_Mysterious, step_faster);
			sound_VolDown_AutoStop(sound_Quake, step_faster);
			sound_VolDown_AutoStop(sound_Magma, step_faster);
			break;
			
		case STATE__FADEOUT:
			vol_Ligt.VolDown(step_slow, 0.0);
			
			sound_VolDown_AutoStop(sound_Mysterious, step_slow);
			sound_VolDown_AutoStop(sound_Quake, step_slow);
			sound_VolDown_AutoStop(sound_Magma, step_slow);
			sound_VolDown_AutoStop(sound_Dooon, step_slow);
			break;
			
		default:
			break;
	}
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
	
	/********************
	********************/
	Gui_Global->gui.draw();
}

/******************************
******************************/
void ofApp::update_ColorOfFire(){
	if(XBC_State.Is_Calm(now))		ColorId_of_Fire = COLOR_ID__CALM; // in case of both Calm and Evil, system choose "Calm".
	else if(XBC_State.Is_Evil(now))	ColorId_of_Fire = COLOR_ID__EVIL;
	else							ColorId_of_Fire = COLOR_ID__BLACK;
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
	
	m.addFloatArg((float)ColorId_of_Fire);
	
	/* */
	Osc_Video_Nebuta.OscSend.sendMessage(m);
	Osc_Video_Wall.OscSend.sendMessage(m);
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
			sprintf(buf, "WAIT");
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
		case STATE__ON:
			sprintf(buf, "ON : push Enter");
			break;
		case STATE__FADEOUT:
			if(d_eachState[State] < now - t_from)	sprintf(buf, "FADEOUT:just a moment please.");
			else									sprintf(buf, "FADEOUT:move the Led away.");
			break;
		default:
			break;
	}
	
	font[FONT_M].drawString(buf, ofGetWidth()/2 - font[FONT_M].stringWidth(buf)/2, ofGetHeight()/2);
	
	/********************
	********************/
	int _width = font[FONT_M].stringWidth("XXXXXXX");
	
	int dt = now - t_from;
	int TimeLeft = (d_eachState[State] - dt)/1000;
	
	if(TimeLeft <= 0)	sprintf(buf, "---");
	else				sprintf(buf, "%5d", TimeLeft);
	
	font[FONT_M].drawString(buf, ofGetWidth() - _width, ofGetHeight() - TextHeight);
	
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
	for(int i = 0; i < NUM_LEDS; i++){
		switch(LedLight[i].LedDeviceType){
			case LED_DEVICE_TYPE_FIXED:
				// ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom + 0 ] = 255; // dimmer
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom + 0 ] = int(255 * Gui_Global->Led_dimmer); // dimmer
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom + 1 ] = ofMap(LedLight[i].LedParam_Out.r * vol_Ligt.get(), 0, 1.0, 0, 255, true);
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom + 2 ] = ofMap(LedLight[i].LedParam_Out.g * vol_Ligt.get(), 0, 1.0, 0, 255, true);
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom + 3 ] = ofMap(LedLight[i].LedParam_Out.b * vol_Ligt.get(), 0, 1.0, 0, 255, true);
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom + 4 ] = ofMap(LedLight[i].LedParam_Out.w * vol_Ligt.get(), 0, 1.0, 0, 255, true);
				ode[ LedLight[i].ODE_id ].universe[ LedLight[i].AddressFrom + 5 ] = 1; // Strobe = open.
				
				break;
				
			case LED_DEVICE_RGB:
				break;
				
			default:
				break;
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
			SetLightPattern__CheckLed();
			break;
			
		case OF_KEY_UP:
			LedId_Test++;
			if(NUM_LEDS <= LedId_Test) LedId_Test = NUM_LEDS;			
			SetLightPattern__CheckLed();
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
