/*
 * Display_Logic.c
 *
 * Created: 7/25/2013 5:48:07 PM
 *  Author: Michael Folz
 */ 

#include "Display_Logic.h"



void Display_Time(int y, int color){
	struct DS1307_Properties *point_time = &Time;
	//struct HT1632C_Properties *point_properties = &Current;
	//point_properties->color= color;
	//point_properties->yPosition= y;
	//DS1307_ReadTime(0,color, DS_Hours);
	///HT1632_Write_Char(12,y, color,':');
	//DS1307_ReadTime(18,color, DS_Minutes);
}

void Display_Date(void){
	struct DS1307_Properties *point_time = &Time;
	DS1307_Copy_Date();
//	HT1632_WriteString(0,2,2,0,point_time->Display_Date);
	for (int clearbuff=0; clearbuff < 100; clearbuff++)point_time->Display_Date[clearbuff]=0x00;
}



