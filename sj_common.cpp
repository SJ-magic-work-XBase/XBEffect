/************************************************************
************************************************************/
#include "sj_common.h"

/************************************************************
************************************************************/
/********************
********************/
int GPIO_0 = 0;
int GPIO_1 = 0;

/********************
********************/
GUI_GLOBAL* Gui_Global = NULL;

FILE* fp_Log = nullptr;


/************************************************************
func
************************************************************/
/******************************
******************************/
double LPF(double LastVal, double CurrentVal, double Alpha_dt, double dt)
{
	double Alpha;
	if((Alpha_dt <= 0) || (Alpha_dt < dt))	Alpha = 1;
	else									Alpha = 1/Alpha_dt * dt;
	
	return CurrentVal * Alpha + LastVal * (1 - Alpha);
}

/******************************
******************************/
double LPF(double LastVal, double CurrentVal, double Alpha)
{
	if(Alpha < 0)		Alpha = 0;
	else if(1 < Alpha)	Alpha = 1;
	
	return CurrentVal * Alpha + LastVal * (1 - Alpha);
}

/******************************
******************************/
double sj_max(double a, double b)
{
	if(a < b)	return b;
	else		return a;
}


/************************************************************
class
************************************************************/

/******************************
******************************/
void GUI_GLOBAL::setup(string GuiName, string FileName, float x, float y)
{
	/********************
	********************/
	gui.setup(GuiName.c_str(), FileName.c_str(), x, y);
	
	/********************
	********************/
	Group_Light.setup("Light");
		ofVec4f ColWhenTest_init = ofVec4f(0.0, 0.0, 1.0, 0.0);
		ofVec4f ColWhenTest_min = ofVec4f(0.0, 0.0, 0.0, 0.0);
		ofVec4f ColWhenTest_max = ofVec4f(1.0, 1.0, 1.0, 1.0);
		Group_Light.add(col_WhenTest.setup("ColTest", ColWhenTest_init, ColWhenTest_min, ColWhenTest_max));
		
		Group_Light.add(Led_dimmer.setup("dimmer", 1.0, 0, 1.0));
		Group_Light.add(volLight_Back_max.setup("L_Back_max", 0.5, 0.01, 1.0)); // XBCamの検知と相談しながら.
	gui.add(&Group_Light);
	
	Group_VolLight.setup("VolLight");
		Group_VolLight.add(Smooth_dt.setup("Smooth_dt", 193, 0, 1500));
		Group_VolLight.add(fft_map_max.setup("fft_map_max", 0.3, 0, 1.0));
	gui.add(&Group_VolLight);
	
	Group_Shutter.setup("Shutter");
		Group_Shutter.add(DmxShutter_open.setup("open", 150, 0, 255));
		Group_Shutter.add(DmxShutter_close.setup("close", 0, 0, 255));
	gui.add(&Group_Shutter);

	/********************
	********************/
	gui.minimizeAll();
}

