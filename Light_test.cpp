/************************************************************
************************************************************/
#include "ofApp.h"
#include "Light.h"

/********************
********************/
ODE ode[] = {
	ODE("10.7.153.16"),
	ODE("10.7.164.56"),
	ODE("10.7.206.7"),
};
const int NUM_ODES = sizeof(ode) / sizeof(ode[0]);

/********************
********************/
DMX_STORAGE DmxShutter(2, 0);

/********************
********************/
const int id_Intro[] = {		
	0	,
	1	,
	5	,
	6	,
	-1	,
};		
		
const int id_QuakeH[] = {		
	2	,
	3	,
	7	,
	8	,
	-1	,
};		
		
const int id_Face[] = {		
	4	,
	9	,
	-1	,
};		

/********************
********************/
LED_LIGHT Light_Dynamic[] = {					//	6					
//				ODE id		Address from		Device			Hardware(Start Address) setting 
/*	0	*/	LED_LIGHT(	0	,	0	,	LED_DEVICE_TYPE_FIXED	),	//	1
/*	1	*/	LED_LIGHT(	0	,	6	,	LED_DEVICE_TYPE_FIXED	),	//	7
/*	2	*/	LED_LIGHT(	0	,	12	,	LED_DEVICE_TYPE_FIXED	),	//	13
/*	3	*/	LED_LIGHT(	0	,	18	,	LED_DEVICE_TYPE_FIXED	),	//	19
/*	4	*/	LED_LIGHT(	0	,	24	,	LED_DEVICE_TYPE_FIXED	),	//	25
/*	5	*/	LED_LIGHT(	0	,	30	,	LED_DEVICE_TYPE_FIXED	),	//	31
/*	6	*/	LED_LIGHT(	0	,	36	,	LED_DEVICE_TYPE_FIXED	),	//	37
/*	7	*/	LED_LIGHT(	0	,	42	,	LED_DEVICE_TYPE_FIXED	),	//	43
/*	8	*/	LED_LIGHT(	0	,	48	,	LED_DEVICE_TYPE_FIXED	),	//	49
/*	9	*/	LED_LIGHT(	0	,	54	,	LED_DEVICE_TYPE_FIXED	),	//	55
};											
											
const int NUM_LIGHTS_DYNAMIC = sizeof(Light_Dynamic) / sizeof(Light_Dynamic[0]);											

/********************
********************/
LED_LIGHT Light_Climax[] = {					//	6					
//				ODE id		Address from		Device			Hardware(Start Address) setting 
/*	0	*/	LED_LIGHT(	1	,	0	,	LED_DEVICE_TYPE_FIXED	),	//	1
/*	1	*/	LED_LIGHT(	1	,	6	,	LED_DEVICE_TYPE_FIXED	),	//	7
/*	2	*/	LED_LIGHT(	1	,	12	,	LED_DEVICE_TYPE_FIXED	),	//	13
/*	3	*/	LED_LIGHT(	1	,	18	,	LED_DEVICE_TYPE_FIXED	),	//	19
/*	4	*/	LED_LIGHT(	1	,	24	,	LED_DEVICE_TYPE_FIXED	),	//	25
};											
											
const int NUM_LIGHTS_CLIMAX = sizeof(Light_Climax) / sizeof(Light_Climax[0]);											

/********************
********************/
LED_LIGHT Light_Back[] = {					//	6						
//				ODE id		Address from		Device			Hardware(Start Address) setting 	
/*	0	*/	LED_LIGHT(	2	,	1	,	LED_DEVICE_TYPE_FIXED	),	//	2	// Address zero is used for DMX shutter.
/*	1	*/	LED_LIGHT(	2	,	7	,	LED_DEVICE_TYPE_FIXED	),	//	8	
};												
												
const int NUM_LIGHTS_BACK = sizeof(Light_Back) / sizeof(Light_Back[0]);												
												
												
LED_LIGHT Light_Volume[] = {					//	6						
//				ODE id		Address from		Device			Hardware(Start Address) setting 	
/*	0	*/	LED_LIGHT(	2	,	13	,	LED_DEVICE_TYPE_FIXED	),	//	14	// same ODE as Light_Back
/*	1	*/	LED_LIGHT(	2	,	19	,	LED_DEVICE_TYPE_FIXED	),	//	20	
};												
												
const int NUM_LIGHTS_VOL = sizeof(Light_Volume) / sizeof(Light_Volume[0]);												
