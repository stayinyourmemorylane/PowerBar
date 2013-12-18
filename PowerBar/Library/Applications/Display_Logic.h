/*
 * Display_Logic.h
 *
 * Created: 7/25/2013 5:53:16 PM
 *  Author: Michael Folz
 */ 


#ifndef DISPLAY_LOGIC_H_
#define DISPLAY_LOGIC_H_

#include "../ClockMemLibs/_AA2432A.h"
#include "../ClockMemLibs/DS1307.h"
#include "../LCD_Libs/LCD_Draw.h"


void Display_Time(int y, int color);
void Display_Date(void);
void Draw_Gui(void);



#endif /* DISPLAY_LOGIC_H_ */