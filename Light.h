/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "ofxArtnet.h"
#include "ofxSetLightPattern.h"

#include "sj_common.h"


/************************************************************
************************************************************/

/**************************************************
**************************************************/
class ODE{
private:
	ofxArtnetSender artnet;	
	char ch_IP[BUF_SIZE_S];
	
public:
	unsigned char universe[SIZE_DMX_UNIVERSE];
	
	ODE(const char* _ch_IP)
	{
		strcpy(ch_IP, _ch_IP);
		artnet.setup(_ch_IP); // Target IP.
		
		ClearUniverse();
	}
	const char* get_IP()	{ return ch_IP; }
	
	void ClearUniverse(){
		for(int i = 0; i < SIZE_DMX_UNIVERSE; i++){
			universe[i] = 0;
		}
	}
	
	void SendDmx(){
		ofxArtnetMessage message;
		message.setData(universe, SIZE_DMX_UNIVERSE);
		artnet.sendArtnet(message);
	}
};

/**************************************************
**************************************************/
class LED_PARAM{
private:
	void _checkLimit(double& val){
		if(val < 0)		val = 0;
		if(1.0 < val)	val = 1.0;
	}
	
	void checkLimit()
	{
		_checkLimit(r);
		_checkLimit(g);
		_checkLimit(b);
		_checkLimit(w);
	}
	

public:
	double r, g, b, w;
	
	LED_PARAM()
	: r(0), g(0), b(0), w(0)
	{
	}
	
	LED_PARAM(double _r, double _g, double _b, double _w)
	: r(_r), g(_g), b(_b), w(_w)
	{
	}
	
	void clear()	{ r = 0; g = 0; b = 0; w = 0; }
	
	LED_PARAM& operator+=(const LED_PARAM& val)
	{
		r += val.r;
		g += val.g;
		b += val.b;
		w += val.w;
		
		checkLimit();
		
		return *this;
	}
	
	LED_PARAM operator+(const LED_PARAM& val) const
	{
		LED_PARAM LedParam = *this;
		
		LedParam.r = LedParam.r + val.r;
		LedParam.g = LedParam.g + val.g;
		LedParam.b = LedParam.b + val.b;
		LedParam.w = LedParam.w + val.w;
		
		LedParam.checkLimit();
		
		return LedParam;
	}
	
	LED_PARAM operator*(double val) const
	{
		LED_PARAM LedParam = *this;
		
		LedParam.r = LedParam.r * val;
		LedParam.g = LedParam.g * val;
		LedParam.b = LedParam.b * val;
		LedParam.w = LedParam.w * val;
		
		LedParam.checkLimit();
		
		return LedParam;
	}
	
	double get_max(){
		double ret = r;
		if(ret < g)	ret = g;
		if(ret < b)	ret = b;
		if(ret < w)	ret = w;
		
		return ret;
	}
};

/**************************************************
**************************************************/
struct LED_LIGHT{
	const int ODE_id;
	const int AddressFrom;
	const enum LED_DEVICE_TYPE LedDeviceType;
	
	LED_PARAM LedParam_Out;
	ofx_LIGHTPATTERN LightPattern_Front;
	ofx_LIGHTPATTERN LightPattern_Back;
	
	LED_LIGHT(int _ODE_id, int _AddressFrom, enum LED_DEVICE_TYPE _LedDeviceType)
	: ODE_id(_ODE_id), AddressFrom(_AddressFrom), LedDeviceType(_LedDeviceType)
	{
		LightPattern_Front.setup(0, 0);
		LightPattern_Back.setup(0, 0);
	}
	
	void clear()
	{
		LedParam_Out.clear();
	}
	
	void update(int now, const LED_PARAM& col_Front, const LED_PARAM& col_Back)
	{
		double Lev = LightPattern_Front.update(now);
		LedParam_Out = col_Front * Lev;
		
		Lev = LightPattern_Back.update(now);
		LedParam_Out += col_Back * Lev;
	}
};


/************************************************************
************************************************************/
extern ODE ode[];
extern const int NUM_ODES;

extern LED_LIGHT Light_Dynamic[];
extern const int NUM_LIGHTS_DYNAMIC;

extern LED_LIGHT* Light_Dynamic_Eye;
extern const int NUM_LIGHTS_EYE;
											
extern LED_LIGHT* Light_Dynamic_FaceUp;
extern const int NUM_LIGHTS_FACE_UP;
											
extern LED_LIGHT* Light_Dynamic_FaceLow;
extern const int NUM_LIGHTS_FACE_LOW;
											
extern LED_LIGHT* Light_Dynamic_ArmUp;
extern const int NUM_LIGHTS_ARM_UP;
											
extern LED_LIGHT* Light_Dynamic_ArmLow;
extern const int NUM_LIGHTS_ARM_LOW;


extern LED_LIGHT Light_Climax[];
extern const int NUM_LIGHTS_CLIMAX;

extern LED_LIGHT Light_Back[];
extern const int NUM_LIGHTS_BACK;



