#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <math.h>
#include "IR_Remote.h"
#include "../AtmegaLibs/AtmegaPins.h"
#include "../AtmegaLibs/Macros_Functions.h"

// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[NUMPULSES][2];  // pair is high and low pulse
uint8_t currentpulse = 0; // index for pulses we're storing

//KGO: added size of compare sample. Only compare the minimum of the two
bool IRcompare(int numpulses, int Signal[], int refsize) {
	int count = min(numpulses,refsize);
	int oncode= 0, offcode = 0;
	
	for (int i=0; i< count-1; i++) {
		
		//pulls the off and on values from the pulse array
		oncode = pulses[i][1] * RESOLUTION / 10;
		offcode = pulses[i+1][0] * RESOLUTION / 10;
		
		// check to make sure the error is less than tollerable
		if ( !( (oncode - Signal[i*2 + 0]) <= (Signal[i*2 + 0] * TOLERANCE / 100))) return false;
		if (!( (offcode - Signal[i*2 + 1]) <= (Signal[i*2 + 1] * TOLERANCE / 100))) return false;
	}
	// Everything matched!
	return true;
}



int IR_Detect(void){
	currentpulse = 0;
	
	while (1){
		uint16_t highpulse, lowpulse;  // temporary storage timing
		highpulse = lowpulse = 0; // start out with no pulse length
		
		while (IRpin_PIN & (1 << IRpin)) {
			// pin is still HIGH
			// count off another few microseconds
			highpulse++;
			_delay_us(RESOLUTION);
			// If the pulse is too long, we 'timed out' - either nothing
			// was received or the code is finished, so print what
			// we've grabbed so far, and then reset
			if (((highpulse >= MAXPULSE) && (currentpulse != 0))|| currentpulse == NUMPULSES) return currentpulse;
		}
		// we didn't time out so lets stash the reading
		pulses[currentpulse][0] = highpulse;
		
		// same as above
		while (! (IRpin_PIN & _BV(IRpin))) {
			// pin is still LOW
			lowpulse++;
			_delay_us(RESOLUTION);
			if (((lowpulse >= MAXPULSE)  && (currentpulse != 0))|| currentpulse == NUMPULSES) 	return currentpulse;
		}
		pulses[currentpulse][1] = lowpulse;
		// we read one high-low pulse successfully, continue!
		currentpulse++;
	}
}

void printpulses(void) {
	printf("\n\n\r\n\rReceived: \n\rOFF \tON");
	for (uint8_t i = 0; i < currentpulse; i++) {
		printf("%d ", pulses[i][0] * RESOLUTION);
		printf(" usec, ");
		printf("%d ", pulses[i][1] * RESOLUTION);
		printf("\n usec");
	}
	
	// print it in a 'array' format
	printf("\nint IRsignal[] = {");
		printf("\n// ON, OFF (in 10's of microseconds)\n");
		for (uint8_t i = 0; i < currentpulse-1; i++) {
			printf("\t"); // tab
			printf("%d ", pulses[i][1] * RESOLUTION / 10);
			//	printf(", ");
			printf("%d ", pulses[i+1][0] * RESOLUTION / 10);
			printf(", \n");
		}
		printf("\t"); // tab
		printf("%d ", pulses[currentpulse-1][1] * RESOLUTION / 10);
	printf(", 0};");
}



int Fire_UP_Button[] = {
	870, 436,
	54, 56,
	52, 56,
	58, 50,
	54, 56,
	52, 50,
	64, 50,
	54, 56,
	52, 56,
	56, 162,
	52, 164,
	54, 164,
	58, 160,
	54, 50,
	62, 160,
	54, 164,
	52, 56,
	54, 56,
	56, 162,
	52, 160,
	58, 54,
	54, 54,
	60, 50,
	52, 58,
	52, 56,
	56, 162,
	52, 54,
	58, 52,
	52, 162,
	62, 160,
	52, 164,
	54, 166,
	56, 160,
	54, 4088,
	868, 220,
54, 0};


int Fire_Down_Button[] = {
	// ON, OFF (in 10's of microseconds)
	868, 438,
	52, 54,
	58, 54,
	50, 58,
	52, 54,
	58, 52,
	52, 58,
	52, 54,
	58, 50,
	54, 166,
	56, 162,
	52, 166,
	52, 164,
	58, 50,
	54, 164,
	58, 160,
	54, 56,
	56, 50,
	54, 56,
	54, 56,
	56, 160,
	54, 54,
	58, 50,
	54, 58,
	52, 54,
	58, 160,
	54, 164,
	52, 164,
	58, 52,
	52, 166,
	58, 160,
	52, 166,
	52, 164,
	58, 4086,
	868, 218,
58, 0};


int Temp_UP_Button[] = {
	// ON, OFF (in 10's of microseconds)
	874, 432,
	58, 52,
	52, 54,
	54, 54,
	60, 50,
	52, 58,
	52, 54,
	58, 52,
	52, 58,
	52, 164,
	52, 164,
	60, 160,
	52, 164,
	54, 56,
	52, 166,
	52, 166,
	56, 52,
	52, 56,
	54, 54,
	58, 52,
	52, 56,
	54, 54,
	58, 52,
	52, 56,
	54, 54,
	58, 160,
	54, 164,
	52, 164,
	58, 162,
	52, 164,
	54, 164,
	58, 160,
	54, 162,
	54, 4092,
	870, 214,
54, 0};

int Temp_Down_Button[] = {
// ON, OFF (in 10's of microseconds)
872, 432,
54, 52,
56, 54,
56, 54,
56, 52,
56, 52,
56, 54,
54, 54,
58, 50,
54, 164,
58, 162,
52, 164,
54, 164,
58, 50,
52, 166,
58, 160,
52, 56,
58, 50,
54, 164,
58, 50,
54, 164,
58, 48,
56, 56,
54, 56,
56, 52,
52, 162,
60, 50,
54, 164,
58, 50,
54, 164,
58, 162,
52, 164,
54, 162,
60, 4084,
870, 216,
60, 0};


int All_ON[] = {
	// ON, OFF (in 10's of microseconds)
	868, 438,
	52, 56,
	56, 52,
	52, 56,
	54, 54,
	58, 52,
	52, 56,
	54, 54,
	58, 52,
	52, 164,
	58, 162,
	52, 162,
	56, 164,
	58, 50,
	54, 164,
	58, 160,
	54, 54,
	58, 160,
	54, 54,
	58, 50,
	54, 164,
	58, 52,
	52, 58,
	52, 54,
	58, 52,
	52, 58,
	52, 162,
	54, 164,
	58, 52,
	52, 166,
	56, 162,
	52, 164,
	54, 164,
	58, 4086,
	868, 216,
60, 0};


int All_OFF[] = {
	// ON, OFF (in 10's of microseconds)
	874, 432,
	58, 50,
	54, 56,
	54, 54,
	58, 52,
	52, 56,
	54, 54,
	58, 50,
	54, 56,
	54, 164,
	52, 166,
	56, 162,
	52, 164,
	52, 58,
	52, 164,
	54, 164,
	58, 50,
	54, 164,
	58, 50,
	54, 164,
	58, 50,
	54, 56,
	54, 54,
	58, 50,
	54, 56,
	54, 54,
	58, 160,
	52, 56,
	58, 160,
	54, 164,
	52, 164,
	58, 162,
	52, 164,
	54, 4084,
	878, 214,
54, 0};