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
		vol += step;
		if(limit < vol) vol = limit;
		checkLimit();
	}
	
	void VolDown(double step, double limit = 0.0){
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
