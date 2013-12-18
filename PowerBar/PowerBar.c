/*
 * NRF24L01_Wireless_RXTX.c
 *
 * Created: 6/11/2013 2:17:18 PM
 *  Author: mfolz
 
 http://learn.adafruit.com/ir-sensor/reading-ir-commands 
 /// PERFECT site for great working code
 */ 
#define F_CPU 16000000UL

#include "Library/LCD_Libs/ST7735_SPI.h"
#include "Library/LCD_Libs/LCD_Draw.h"

#include "Library/Applications/Memory_Logic.h"
#include "Library/Applications/Terminal.h"

#include "Library/AtmegaLibs/AtmegaPins.h"
#include "Library/AtmegaLibs/Serial_Atmel328p.h"
#include "Library/AtmegaLibs/SPI.h"
#include "Library/NRF/NRF24.h"
#include "Library/Applications/Powerbar.h"
#include "Library/Applications/Communications.h"
#include "Library/IR/IR_Remote.h"
#include "Library/ClockMemLibs/DS1307.h"
#include "Library/ClockMemLibs/_AA2432A.h"

#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/pgmspace.h>  // this is needed for the program memory
#include <util/delay.h>  // Delay

#define POWERBAR_DDR DDRC
#define POWERBAR_PORT PORTC
#define LIGHT_PIN PORTC1
#define FAN_PIN PORTC2

void IR_Routine(void); 

int main(void)
{
	struct LCD_Geometry *pointCurrent = &Current;
	struct LCD_Properties *pointProperties = &Properties;
	FILE lcd_str = FDEV_SETUP_STREAM(TerminalChar, NULL, _FDEV_SETUP_RW);
	stdout = &lcd_str;
	USART_init();
	ST7735_Properties(128,160,18,'T');
	ST7735_Initalize();
	pointProperties->Rotation = 2;	
	terminalemulator();
	Terminal_Set_Properties(2, 1, 0xFFFFFF);

	
	CLEARBIT(IR_PIN_DDR, IR_Input); // make input 
	POWERBAR_DDR |= (1 << LIGHT_PIN) | (1 << PORTC2); 	
	I2C_Initalize();

	
	DS1307_SetTime(0,51,22,5,25,6, 2013);
	
	Draw_Gui();

	while(1){
	   IR_Routine();
	}
}



/*
THESE BOTH WORK 100% 
Server_Application();
Client_Application();
*/ 