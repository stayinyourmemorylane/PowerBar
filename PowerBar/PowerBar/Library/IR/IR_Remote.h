/*
 * IR_Remote.h
 *
 * Created: 7/3/2013 11:35:32 PM
 *  Author: Michael Folz
 */ 


#ifndef IR_REMOTE_H_INCLUDED
#define IR_REMOTE_H_INCLUDED


#define IR_Input PORTC0 // input for ir
#define IR_PIN_DDR DDRC   // data direction port for the pins
#define IR_PORT PORTC

#define IRpin_PIN      PINC
#define IRpin          0
#define MAXPULSE 35000  // max high width anything above this will be eliminated
// what our timing resolution should be, larger is better
// as its more 'precise' - but too large and you wont get
// accurate timing
#define RESOLUTION 20
#define NUMPULSES 36
// What percent we will allow in variation to match the same code
#define TOLERANCE 20




void IR_RUN(void);
int IR_Detect(void);
void IR_PrintPulse(void);
void printpulses(void);
bool IRcompare(int numpulses, int Signal[], int refsize);
min(int value1, int value2 ); 

// CAR MP3 REMOTE
int CH_MINUS[];
int CH_Button[];
int CH_PLUS[];
int Previous_Button[]; 
int PWR[];
int UP_Button[];
int Down_button[]; 
int Right_button[];
int Left_Button[];

int Fire_UP_Button[];
int Fire_Down_Button[];
int Temp_Down_Button[];
int Temp_UP_Button[];

int All_ON[];
int All_OFF[];

#endif /* IR_REMOTE_H_ */