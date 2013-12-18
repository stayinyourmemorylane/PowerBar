/*
 * DS1307.h
 *
 * Created: 3/12/2013 11:45:26 AM
 *  Author: mfolz
 */ 
/// DS1307 Clock module 

#ifndef DS1307_H_INCLUDED_
#define DS1307_H_INCLUDED_

#include "../AtmegaLibs/Macros_Functions.h"

#define F_CPU 16000000UL
#include <avr/delay.h>


#define	DS_Seconds	0x00
#define DS_Minutes	0x01
#define DS_Hours	0x02
#define	DS_WeekDay	0x03
#define	DS_Date		0x04
#define	DS_Month	0x05
#define DS_Year		0x06
#define DS_ReadTime_Delay _delay_ms(50);


void DS1307_Copy_Date();
void DS1307_ReadTime(int x, int color, int address);
void DS1307_SetTime(int seconds, int minutes, int hours, int weekday, int date, int months, int year);
void DS1307_WriteTime(int address, int time);

struct DS1307_Properties{
	int CurrentTime;
	int Seconds;
	int Day;
	int Seconds_Compare;
	char Display_Date[100];
}Time;



#endif /* DS1307_H_ */