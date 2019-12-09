/************************************************************
************************************************************/
#include "ofApp.h"
#include "Light.h"

/********************
********************/
ODE ode[] = {
	ODE("10.7.184.202"),
	ODE("10.8.4.126"),
	ODE("10.7.206.7"),
};
const int NUM_ODES = sizeof(ode) / sizeof(ode[0]);

/********************
********************/
DMX_STORAGE DmxShutter(2, 0);

/********************
********************/
const int id_Intro[] = {		
	5	,
	6	,
	7	,
	8	,
	9	,
	10	,
	11	,
	13	,
	14	,
	15	,
	16	,
	17	,
	18	,
	19	,
	20	,
	21	,
	42	,
	43	,
	44	,
	45	,
	46	,
	47	,
	48	,
	49	,
	50	,
	51	,
	52	,
	53	,
	54	,
	55	,
	60	,
	61	,
	62	,
	63	,
	64	,
	65	,
	66	,
	67	,
	100	,
	101	,
	102	,
	103	,
	110	,
	111	,
	112	,
	113	,
	-1	,
};		
		
const int id_QuakeH[] = {		
	0	,
	1	,
	2	,
	3	,
	4	,
	22	,
	23	,
	34	,
	35	,
	36	,
	37	,
	38	,
	39	,
	40	,
	41	,
	68	,
	69	,
	70	,
	71	,
	72	,
	104	,
	105	,
	106	,
	107	,
	114	,
	115	,
	116	,
	117	,
	118	,
	119	,
	120	,
	121	,
	122	,
	123	,
	124	,
	125	,
	126	,
	127	,
	128	,
	129	,
	-1	,
};		
		
const int id_Face[] = {		
	24	,
	25	,
	26	,
	27	,
	28	,
	29	,
	30	,
	31	,
	32	,
	33	,
	-1	,
};		

/********************
********************/
LED_LIGHT Light_Dynamic[] = {					//	3					
//				ODE id		Address from		Device			Hardware(Start Address) setting 
/*	0	*/	LED_LIGHT(	0	,	0	,	LED_DEVICE_RGB	),	//	1
/*	1	*/	LED_LIGHT(	0	,	3	,	LED_DEVICE_RGB	),	//	4
/*	2	*/	LED_LIGHT(	0	,	6	,	LED_DEVICE_RGB	),	//	7
/*	3	*/	LED_LIGHT(	0	,	9	,	LED_DEVICE_RGB	),	//	10
/*	4	*/	LED_LIGHT(	0	,	12	,	LED_DEVICE_RGB	),	//	13
/*	5	*/	LED_LIGHT(	0	,	15	,	LED_DEVICE_RGB	),	//	16
/*	6	*/	LED_LIGHT(	0	,	18	,	LED_DEVICE_RGB	),	//	19
/*	7	*/	LED_LIGHT(	0	,	21	,	LED_DEVICE_RGB	),	//	22
/*	8	*/	LED_LIGHT(	0	,	24	,	LED_DEVICE_RGB	),	//	25
/*	9	*/	LED_LIGHT(	0	,	27	,	LED_DEVICE_RGB	),	//	28
/*	10	*/	LED_LIGHT(	0	,	30	,	LED_DEVICE_RGB	),	//	31
/*	11	*/	LED_LIGHT(	0	,	33	,	LED_DEVICE_RGB	),	//	34
/*	12	*/	LED_LIGHT(	0	,	36	,	LED_DEVICE_RGB	),	//	37
/*	13	*/	LED_LIGHT(	0	,	39	,	LED_DEVICE_RGB	),	//	40
/*	14	*/	LED_LIGHT(	0	,	42	,	LED_DEVICE_RGB	),	//	43
/*	15	*/	LED_LIGHT(	0	,	45	,	LED_DEVICE_RGB	),	//	46
/*	16	*/	LED_LIGHT(	0	,	48	,	LED_DEVICE_RGB	),	//	49
/*	17	*/	LED_LIGHT(	0	,	51	,	LED_DEVICE_RGB	),	//	52
/*	18	*/	LED_LIGHT(	0	,	54	,	LED_DEVICE_RGB	),	//	55
/*	19	*/	LED_LIGHT(	0	,	57	,	LED_DEVICE_RGB	),	//	58
/*	20	*/	LED_LIGHT(	0	,	60	,	LED_DEVICE_RGB	),	//	61
/*	21	*/	LED_LIGHT(	0	,	63	,	LED_DEVICE_RGB	),	//	64
/*	22	*/	LED_LIGHT(	0	,	66	,	LED_DEVICE_RGB	),	//	67
/*	23	*/	LED_LIGHT(	0	,	69	,	LED_DEVICE_RGB	),	//	70
/*	24	*/	LED_LIGHT(	0	,	72	,	LED_DEVICE_RGB	),	//	73
/*	25	*/	LED_LIGHT(	0	,	75	,	LED_DEVICE_RGB	),	//	76
/*	26	*/	LED_LIGHT(	0	,	78	,	LED_DEVICE_RGB	),	//	79
/*	27	*/	LED_LIGHT(	0	,	81	,	LED_DEVICE_RGB	),	//	82
/*	28	*/	LED_LIGHT(	0	,	84	,	LED_DEVICE_RGB	),	//	85
/*	29	*/	LED_LIGHT(	0	,	87	,	LED_DEVICE_RGB	),	//	88
/*	30	*/	LED_LIGHT(	0	,	90	,	LED_DEVICE_RGB	),	//	91
/*	31	*/	LED_LIGHT(	0	,	93	,	LED_DEVICE_RGB	),	//	94
/*	32	*/	LED_LIGHT(	0	,	96	,	LED_DEVICE_RGB	),	//	97
/*	33	*/	LED_LIGHT(	0	,	99	,	LED_DEVICE_RGB	),	//	100
/*	34	*/	LED_LIGHT(	0	,	102	,	LED_DEVICE_RGB	),	//	103
/*	35	*/	LED_LIGHT(	0	,	105	,	LED_DEVICE_RGB	),	//	106
/*	36	*/	LED_LIGHT(	0	,	108	,	LED_DEVICE_RGB	),	//	109
/*	37	*/	LED_LIGHT(	0	,	111	,	LED_DEVICE_RGB	),	//	112
/*	38	*/	LED_LIGHT(	0	,	114	,	LED_DEVICE_RGB	),	//	115
/*	39	*/	LED_LIGHT(	0	,	117	,	LED_DEVICE_RGB	),	//	118
/*	40	*/	LED_LIGHT(	0	,	120	,	LED_DEVICE_RGB	),	//	121
/*	41	*/	LED_LIGHT(	0	,	123	,	LED_DEVICE_RGB	),	//	124
/*	42	*/	LED_LIGHT(	0	,	126	,	LED_DEVICE_RGB	),	//	127
/*	43	*/	LED_LIGHT(	0	,	129	,	LED_DEVICE_RGB	),	//	130
/*	44	*/	LED_LIGHT(	0	,	132	,	LED_DEVICE_RGB	),	//	133
/*	45	*/	LED_LIGHT(	0	,	135	,	LED_DEVICE_RGB	),	//	136
/*	46	*/	LED_LIGHT(	0	,	138	,	LED_DEVICE_RGB	),	//	139
/*	47	*/	LED_LIGHT(	0	,	141	,	LED_DEVICE_RGB	),	//	142
/*	48	*/	LED_LIGHT(	0	,	144	,	LED_DEVICE_RGB	),	//	145
/*	49	*/	LED_LIGHT(	0	,	147	,	LED_DEVICE_RGB	),	//	148
/*	50	*/	LED_LIGHT(	0	,	150	,	LED_DEVICE_RGB	),	//	151
/*	51	*/	LED_LIGHT(	0	,	153	,	LED_DEVICE_RGB	),	//	154
/*	52	*/	LED_LIGHT(	0	,	156	,	LED_DEVICE_RGB	),	//	157
/*	53	*/	LED_LIGHT(	0	,	159	,	LED_DEVICE_RGB	),	//	160
/*	54	*/	LED_LIGHT(	0	,	162	,	LED_DEVICE_RGB	),	//	163
/*	55	*/	LED_LIGHT(	0	,	165	,	LED_DEVICE_RGB	),	//	166
/*	56	*/	LED_LIGHT(	0	,	168	,	LED_DEVICE_RGB	),	//	169
/*	57	*/	LED_LIGHT(	0	,	171	,	LED_DEVICE_RGB	),	//	172
/*	58	*/	LED_LIGHT(	0	,	174	,	LED_DEVICE_RGB	),	//	175
/*	59	*/	LED_LIGHT(	0	,	177	,	LED_DEVICE_RGB	),	//	178
/*	60	*/	LED_LIGHT(	0	,	180	,	LED_DEVICE_RGB	),	//	181
/*	61	*/	LED_LIGHT(	0	,	183	,	LED_DEVICE_RGB	),	//	184
/*	62	*/	LED_LIGHT(	0	,	186	,	LED_DEVICE_RGB	),	//	187
/*	63	*/	LED_LIGHT(	0	,	189	,	LED_DEVICE_RGB	),	//	190
/*	64	*/	LED_LIGHT(	0	,	192	,	LED_DEVICE_RGB	),	//	193
/*	65	*/	LED_LIGHT(	0	,	195	,	LED_DEVICE_RGB	),	//	196
/*	66	*/	LED_LIGHT(	0	,	198	,	LED_DEVICE_RGB	),	//	199
/*	67	*/	LED_LIGHT(	0	,	201	,	LED_DEVICE_RGB	),	//	202
/*	68	*/	LED_LIGHT(	0	,	204	,	LED_DEVICE_RGB	),	//	205
/*	69	*/	LED_LIGHT(	0	,	207	,	LED_DEVICE_RGB	),	//	208
/*	70	*/	LED_LIGHT(	0	,	210	,	LED_DEVICE_RGB	),	//	211
/*	71	*/	LED_LIGHT(	0	,	213	,	LED_DEVICE_RGB	),	//	214
/*	72	*/	LED_LIGHT(	0	,	216	,	LED_DEVICE_RGB	),	//	217
/*	73	*/	LED_LIGHT(	0	,	219	,	LED_DEVICE_RGB	),	//	220
/*	74	*/	LED_LIGHT(	0	,	222	,	LED_DEVICE_RGB	),	//	223
/*	75	*/	LED_LIGHT(	0	,	225	,	LED_DEVICE_RGB	),	//	226
/*	76	*/	LED_LIGHT(	0	,	228	,	LED_DEVICE_RGB	),	//	229
/*	77	*/	LED_LIGHT(	0	,	231	,	LED_DEVICE_RGB	),	//	232
/*	78	*/	LED_LIGHT(	0	,	234	,	LED_DEVICE_RGB	),	//	235
/*	79	*/	LED_LIGHT(	0	,	237	,	LED_DEVICE_RGB	),	//	238
/*	80	*/	LED_LIGHT(	0	,	240	,	LED_DEVICE_RGB	),	//	241
/*	81	*/	LED_LIGHT(	0	,	243	,	LED_DEVICE_RGB	),	//	244
/*	82	*/	LED_LIGHT(	0	,	246	,	LED_DEVICE_RGB	),	//	247
/*	83	*/	LED_LIGHT(	0	,	249	,	LED_DEVICE_RGB	),	//	250
/*	84	*/	LED_LIGHT(	0	,	252	,	LED_DEVICE_RGB	),	//	253
/*	85	*/	LED_LIGHT(	0	,	255	,	LED_DEVICE_RGB	),	//	256
/*	86	*/	LED_LIGHT(	0	,	258	,	LED_DEVICE_RGB	),	//	259
/*	87	*/	LED_LIGHT(	0	,	261	,	LED_DEVICE_RGB	),	//	262
/*	88	*/	LED_LIGHT(	0	,	264	,	LED_DEVICE_RGB	),	//	265
/*	89	*/	LED_LIGHT(	0	,	267	,	LED_DEVICE_RGB	),	//	268
/*	90	*/	LED_LIGHT(	0	,	270	,	LED_DEVICE_RGB	),	//	271
/*	91	*/	LED_LIGHT(	0	,	273	,	LED_DEVICE_RGB	),	//	274
/*	92	*/	LED_LIGHT(	0	,	276	,	LED_DEVICE_RGB	),	//	277
/*	93	*/	LED_LIGHT(	0	,	279	,	LED_DEVICE_RGB	),	//	280
/*	94	*/	LED_LIGHT(	0	,	282	,	LED_DEVICE_RGB	),	//	283
/*	95	*/	LED_LIGHT(	0	,	285	,	LED_DEVICE_RGB	),	//	286
/*	96	*/	LED_LIGHT(	0	,	288	,	LED_DEVICE_RGB	),	//	289
/*	97	*/	LED_LIGHT(	0	,	291	,	LED_DEVICE_RGB	),	//	292
/*	98	*/	LED_LIGHT(	0	,	294	,	LED_DEVICE_RGB	),	//	295
/*	99	*/	LED_LIGHT(	0	,	297	,	LED_DEVICE_RGB	),	//	298
/*	100	*/	LED_LIGHT(	0	,	300	,	LED_DEVICE_RGB	),	//	301
/*	101	*/	LED_LIGHT(	0	,	303	,	LED_DEVICE_RGB	),	//	304
/*	102	*/	LED_LIGHT(	0	,	306	,	LED_DEVICE_RGB	),	//	307
/*	103	*/	LED_LIGHT(	0	,	309	,	LED_DEVICE_RGB	),	//	310
/*	104	*/	LED_LIGHT(	0	,	312	,	LED_DEVICE_RGB	),	//	313
/*	105	*/	LED_LIGHT(	0	,	315	,	LED_DEVICE_RGB	),	//	316
/*	106	*/	LED_LIGHT(	0	,	318	,	LED_DEVICE_RGB	),	//	319
/*	107	*/	LED_LIGHT(	0	,	321	,	LED_DEVICE_RGB	),	//	322
/*	108	*/	LED_LIGHT(	0	,	324	,	LED_DEVICE_RGB	),	//	325
/*	109	*/	LED_LIGHT(	0	,	327	,	LED_DEVICE_RGB	),	//	328
/*	110	*/	LED_LIGHT(	0	,	330	,	LED_DEVICE_RGB	),	//	331
/*	111	*/	LED_LIGHT(	0	,	333	,	LED_DEVICE_RGB	),	//	334
/*	112	*/	LED_LIGHT(	0	,	336	,	LED_DEVICE_RGB	),	//	337
/*	113	*/	LED_LIGHT(	0	,	339	,	LED_DEVICE_RGB	),	//	340
/*	114	*/	LED_LIGHT(	0	,	342	,	LED_DEVICE_RGB	),	//	343
/*	115	*/	LED_LIGHT(	0	,	345	,	LED_DEVICE_RGB	),	//	346
/*	116	*/	LED_LIGHT(	0	,	348	,	LED_DEVICE_RGB	),	//	349
/*	117	*/	LED_LIGHT(	0	,	351	,	LED_DEVICE_RGB	),	//	352
/*	118	*/	LED_LIGHT(	0	,	354	,	LED_DEVICE_RGB	),	//	355
/*	119	*/	LED_LIGHT(	0	,	357	,	LED_DEVICE_RGB	),	//	358
/*	120	*/	LED_LIGHT(	0	,	360	,	LED_DEVICE_RGB	),	//	361
/*	121	*/	LED_LIGHT(	0	,	363	,	LED_DEVICE_RGB	),	//	364
/*	122	*/	LED_LIGHT(	0	,	366	,	LED_DEVICE_RGB	),	//	367
/*	123	*/	LED_LIGHT(	0	,	369	,	LED_DEVICE_RGB	),	//	370
/*	124	*/	LED_LIGHT(	0	,	372	,	LED_DEVICE_RGB	),	//	373
/*	125	*/	LED_LIGHT(	0	,	375	,	LED_DEVICE_RGB	),	//	376
/*	126	*/	LED_LIGHT(	0	,	378	,	LED_DEVICE_RGB	),	//	379
/*	127	*/	LED_LIGHT(	0	,	381	,	LED_DEVICE_RGB	),	//	382
/*	128	*/	LED_LIGHT(	0	,	384	,	LED_DEVICE_RGB	),	//	385
/*	129	*/	LED_LIGHT(	0	,	387	,	LED_DEVICE_RGB	),	//	388
};											
											
const int NUM_LIGHTS_DYNAMIC = sizeof(Light_Dynamic) / sizeof(Light_Dynamic[0]);											
											

/********************
********************/
LED_LIGHT Light_Climax[] = {					//	1					
//				ODE id		Address from		Device			Hardware(Start Address) setting 
/*	0	*/	LED_LIGHT(	1	,	0	,	LED_DEVICE_1CH	),	//	1
// /*	1	*/	LED_LIGHT(	1	,	1	,	LED_DEVICE_1CH	),	//	2
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
