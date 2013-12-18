/*
 * _24AA32A.h
 *
 * Created: 3/13/2013 1:24:03 PM
 *  Author: mfolz
 Works with most ATmega memory modules _24C32 
 
 */ 


#ifndef _24AA32A_H_INCLUDED_
#define _24AA32A_H_INCLUDED_

#define  F_CPU    16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define Device_Address 0xA0
#define ReadMode 0x01
#define WriteBit 31
#define DataInDelay() _delay_ms(50) // this is necessary to wait after each write cycle. 
#define DataOutDelay() _delay_us(10) // this is necessary to wait after each read cycle.
/// 32k Serial EEPROM


/// Bad function D: this is crappy >: 
void _24AA32A_ReadMultipleBytes_Slow(int byte_address, int length_read, char *store_data);
void _24AA32A_SendByte(int byte_address, char byte);
void _24AA32A_Send32Bytes(int byte_address, char length, char *byte);
void _24AA32A_ReadMultipleBytes_Slow(int byte_address, int length_read, char *store_data);
void _24AA32A_SendMultipleBytes_Slow(int byte_address, char total_length, char *byte);
char _24AA32A_ReadByte(int byte_address);




#endif 


