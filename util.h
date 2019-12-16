/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "sj_common.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
// class SOUND_PAIR : private Noncopyable{
class SOUND_PAIR{
private:
	ofSoundPlayer sound_Calm;
	ofSoundPlayer sound_Evil;
	
	int PosStartMs_Calm;
	int PosStartMs_Evil;
	
	/******************************
	AutoStartしない : Calm/Evil 両方鳴ってしまうので.
	******************************/
	void sound_VolUp(ofSoundPlayer& sound, double step, double limit = 1.0)
	{
		if(!sound.isLoaded() || !sound.isPlaying()) return;
		
		float vol = sound.getVolume();
		vol += step;
		if(limit < vol) vol = limit;
		sound.setVolume(vol);
	}
	
	void sound_VolDown_AutoStop(ofSoundPlayer& sound, double step, double limit = 0.0)
	{
		if(!sound.isLoaded() || !sound.isPlaying()) return;
		
		float vol = sound.getVolume();
		vol -= step;
		if(vol < limit) vol = limit;
		sound.setVolume(vol);
		
		if( (vol <= 0) && (sound.isPlaying()) ){
			sound.stop();
			sound.setPosition(0);
		}
	}
	
public:
	SOUND_PAIR()
	{
	}
	
	SOUND_PAIR(string FileName_Calm, string FileName_Evil, int _PosStartMs_Calm, int _PosStartMs_Evil, bool b_Loop, double vol)
	: PosStartMs_Calm(_PosStartMs_Calm)
	, PosStartMs_Evil(_PosStartMs_Evil)
	{
		/* */
		sound_Calm.load(FileName_Calm.c_str());
		if(sound_Calm.isLoaded()){
			sound_Calm.setLoop(b_Loop);
			sound_Calm.setMultiPlay(true);
			sound_Calm.setVolume(vol);
		}else{
			printf("%s load Error\n", FileName_Calm.c_str());
			fflush(stdout);
		}
		
		/* */
		sound_Evil.load(FileName_Evil.c_str());
		if(sound_Evil.isLoaded()){
			sound_Evil.setLoop(b_Loop);
			sound_Evil.setMultiPlay(true);
			sound_Evil.setVolume(vol);
		}else{
			printf("%s load Error\n", FileName_Evil.c_str());
			fflush(stdout);
		}
	}
	
	bool isPlaying(){
		if( (sound_Calm.isPlaying()) || (sound_Evil.isPlaying()) )	return true;
		else														return false;
	}
	
	void stop(){
		if(sound_Calm.isPlaying())	sound_Calm.stop();
		if(sound_Evil.isPlaying())	sound_Evil.stop();
	}
	
	void play(bool b_Calm){
		if(b_Calm)	sound_Calm.play();
		else		sound_Evil.play();
	}
	
	void setVolume(double val){
		sound_Calm.setVolume(val);
		sound_Evil.setVolume(val);
	}
	
	double getVolume(){
		if(sound_Calm.isPlaying())		return sound_Calm.getVolume();
		else if(sound_Evil.isPlaying())	return sound_Evil.getVolume();
		else							return 0;
	}
	
	void setStartPositionMS(){
		sound_Calm.setPositionMS(PosStartMs_Calm);
		sound_Evil.setPositionMS(PosStartMs_Evil);
	}
	
	void VolUp(double step, double limit = 1.0){
		sound_VolUp(sound_Calm, step, limit);
		sound_VolUp(sound_Evil, step, limit);
	}
	
	void VolDown(double step, double limit = 0.0){
		sound_VolDown_AutoStop(sound_Calm, step, limit);
		sound_VolDown_AutoStop(sound_Evil, step, limit);
	}
	
	/*
	ofSoundPlayer& getSound(bool b_Calm){
		if(b_Calm)	return sound_Calm;
		else		return sound_Evil;
	}
	*/
};

/**************************************************
**************************************************/
class VOLUME : private Noncopyable{
private:
	double vol = 0;
	
	void checkLimit(){
		if(vol < 0)		vol = 0;
		if(1.0 < vol)	vol = 1.0;
	}

public:
	void set(double _val)	{ vol = _val; checkLimit(); }
	double get()			{ return vol; }
	
	void VolUp(double step, double limit = 1.0){
		if(limit <= 0)	{ ERROR_MSG(); std::exit(1); } // must be coding Error.
		
		vol += step;
		if(limit < vol) vol = limit;
		checkLimit();
	}
	
	void VolDown(double step, double limit = 0.0){
		if(1 <=limit)	{ ERROR_MSG(); std::exit(1); } // must be coding Error.
		
		vol -= step;
		if(vol < limit) vol = limit;
		checkLimit();
	}
};

/**************************************************
**************************************************/
class BOOL_WITH_EXPIRATION_DATE{
private:
	const int d_Expiration_ms;
	int t_LastUpdate_ms = 0;
	bool b_val = false;
	
public:
	BOOL_WITH_EXPIRATION_DATE(int _d_Expiration_ms = 1000)
	: d_Expiration_ms(_d_Expiration_ms)
	{
	}
	
	void set(int now, bool _val){
		b_val = _val;
		t_LastUpdate_ms = now;
	}
	
	bool get(int now){
		if(d_Expiration_ms < now - t_LastUpdate_ms)	return false;
		else										return b_val;
	}
};

/**************************************************
**************************************************/
class XBC_STATE : private Noncopyable{
private:
	BOOL_WITH_EXPIRATION_DATE be_Osc_Calm;
	bool b_key_Calm = false;
	
	BOOL_WITH_EXPIRATION_DATE be_Osc_Evil;
	bool b_key_Evil = false;
	
	BOOL_WITH_EXPIRATION_DATE be_Osc_Motion;
	bool b_key_Motion = false;
	
public:
	XBC_STATE()
	{
	}
	
	void update_osc(int now, bool _b_Calm, bool _b_Evil, bool _b_Motion){
		be_Osc_Calm.set(now, _b_Calm);
		be_Osc_Evil.set(now, _b_Evil);
		be_Osc_Motion.set(now, _b_Motion);
	}
	
	void KeyIn_Calm(bool val)	{ b_key_Calm = val; }
	void KeyIn_Evil(bool val)	{ b_key_Evil = val; }
	void KeyIn_Motion(bool val) { b_key_Motion = val; }
	
	void KeyToggle_Calm()	{ b_key_Calm = !b_key_Calm; }
	void KeyToggle_Evil()	{ b_key_Evil = !b_key_Evil; }
	void KeyToggle_Motion() { b_key_Motion = !b_key_Motion; }
	
	
	bool Is_LedOn(int now){
		if( Is_Calm(now) || Is_Evil(now) )	return true;
		else								return false;
	}
	
	bool Is_Calm(int now){
		if( (be_Osc_Calm.get(now)) || (b_key_Calm) )	return true;
		else											return false;
	}
	
	bool Is_Evil(int now){
		if( (be_Osc_Evil.get(now)) || (b_key_Evil) )	return true;
		else											return false;
	}
	
	bool Is_Motion(int now){
		if( (be_Osc_Motion.get(now)) || (b_key_Motion) )	return true;
		else												return false;
	}	
};
