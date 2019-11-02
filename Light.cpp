/************************************************************
************************************************************/
#include "ofApp.h"
#include "Light.h"

/********************
********************/
ODE ode[] = {
	ODE("10.7.153.16"),
	ODE("10.7.164.56"),
};
const int NUM_ODES = sizeof(ode) / sizeof(ode[0]);

/********************
********************/
LED_LIGHT LedLight[] = {											
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
/*	10	*/	LED_LIGHT(	1	,	0	,	LED_DEVICE_TYPE_FIXED	),	//	1
/*	11	*/	LED_LIGHT(	1	,	6	,	LED_DEVICE_TYPE_FIXED	),	//	7
/*	12	*/	LED_LIGHT(	1	,	12	,	LED_DEVICE_TYPE_FIXED	),	//	13
/*	13	*/	LED_LIGHT(	1	,	18	,	LED_DEVICE_TYPE_FIXED	),	//	19
/*	14	*/	LED_LIGHT(	1	,	24	,	LED_DEVICE_TYPE_FIXED	),	//	25
/*	15	*/	LED_LIGHT(	1	,	30	,	LED_DEVICE_TYPE_FIXED	),	//	31
/*	16	*/	LED_LIGHT(	1	,	36	,	LED_DEVICE_TYPE_FIXED	),	//	37
/*	17	*/	LED_LIGHT(	1	,	42	,	LED_DEVICE_TYPE_FIXED	),	//	43
/*	18	*/	LED_LIGHT(	1	,	48	,	LED_DEVICE_TYPE_FIXED	),	//	49
/*	19	*/	LED_LIGHT(	1	,	54	,	LED_DEVICE_TYPE_FIXED	),	//	55
};
											
const int NUM_LEDS = sizeof(LedLight) / sizeof(LedLight[0]);											