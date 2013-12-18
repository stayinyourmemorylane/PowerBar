/*
 * Memory_Logic.h
 *
 * Created: 3/24/2013 4:38:42 PM
 *  Author: Michael
 */ 


#ifndef MEMORY_LOGIC_H_
#define MEMORY_LOGIC_H_

#include "../AtmegaLibs/I2C.h"
#include "../ClockMemLibs/_AA2432A.h"


/// Define the memory sections here
//1 = Quotes -> 0x0000 - 0x09FF-  2559 chars
///  UP TO - 63 Strings can be stored successfully 
#define Memory_String_Section 0x0100 // All strings are stored here with a null terminator at the end
#define Memory_String_Location_Begin 0x0002 // Location + Length are stored here until 0x00FF 
#define Memory_String_Number 0x0000 // from 0x0000-0x0001 the number of strings is stored here. 256 
#define Memory_String_End 0x09FF // End of strings
//2 =  Com Strings -> 0x0A00- 0x1000 - 1536 chars
/// - 63 Strings can be stored successfully 
#define Memory_Com_String_Section 0x0B00 // 
#define Memory_Com_Location_Begin 0x0A02 //Similar to the String location give 2 bytes between number and location
#define Memory_Com_String_Number 0x0A00 // Similar to String number 
#define Memory_Com_String_End 0x0FFF // End 


struct Memory_Application_Info{
	char String_Number;   
	int String_Length;
  	int String_Location; 
	int String_Lenght_Count; 
	int Memory_Message_Location_Begin;
	int Memory_Message_Number;
	int Memory_Message_Section;
	int Memory_Message_Section_End;
	int String_Inc;
	char Read_String[255];
}StringInfo;




void WriteString2Memory(char *stringin, char String_Type);  // max 256 strings
// Read string from memory location check to see if number is valid pull up information and display
char *ReadStringMemory(char stringnumber,char String_Type);

void Memtest(void);
void WriteEasyMem(int location);
void EASYStart(void);
void WriteAllStrings(void);



void StoreFont(void);
char *ReadFont(int font_value);


#endif /* MEMORY_LOGIC_H_ */