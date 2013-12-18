/*
 * PowerBar.h
 *
 * Created: 7/3/2013 3:14:02 PM
 *  Author: mfolz
 */ 


#ifndef POWERBAR_H_
#define POWERBAR_H_

#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>
#include "Communications.h"
#include "../LCD_Libs/LCD_Draw.h"
#include "../AtmegaLibs/Serial_Atmel328p.h"
#include "../AtmegaLibs/SPI.h"
#include "../NRF/NRF24.h"
#include "../AtmegaLibs/AtmegaPins.h"
#include "../IR/IR_Remote.h"
#include "../ClockMemLibs/DS1307.h"


#define POWERBAR_DDR DDRC
#define POWERBAR_PORT PORTC
#define LIGHT_PIN PORTC1
#define FAN_PIN PORTC2

#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define FLIPBIT(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))
#define CHECKBIT(ADDRESS,BIT) (ADDRESS & (1<<BIT))





void PowerBar_Application(void);  // start ports and set status for client module 
void PowerBar_DeviceManagement(char device); // turn on devices - turn others off
void PowerBar_SendStatus(void);   // Send the current status of the devices attached to the power bar. 



void Draw_Gui(void);
void IR_Routine(void);




#endif /* POWERBAR_H_ */