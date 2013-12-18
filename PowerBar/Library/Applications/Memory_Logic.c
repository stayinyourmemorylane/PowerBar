/*
 * Memory_Logic.c
 *
 * Created: 4/29/2013 10:18:49 AM
 *  Author: mfolz
 */ 



#include "Memory_Logic.h"
#define  F_CPU    16000000UL
#include <avr/io.h>
#include <util/delay.h>


/// Display Library // Display both Hours and minutes // should make this variable.
char *Douglass[] = {
"I'm not mad, I'm proud of you. You took your first pinch like a man and you learn two great things in your life. Look at me, never rat on your friends and always keep your mouth shut. - Goodfellas",
"If real is what you can feel, smell, taste and see, then 'real' is simply electrical signals interpreted by your brain  - Matrix ",
"Throughout human history, we have been dependent on machines to survive. Fate, it seems, is not without a sense of irony. - Matrix",
"Sooner or later you're going to realize just as I did that there's a difference between knowing the path and walking the path. - Matrix",
};

char *Clock_happy[] =
{
"Thi255125s is a test",
"Th12512561is is also a test",
"TEST!!!!                 were u surprised? ", // blank....
"TEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEST", // blank....
"I have no idea what i am doing",
"TESTING TEST TESTING TESTERS TESTS",
 "You can go away now"
};  


void Memtest(void){
	struct Memory_Application_Info *Mem_Var = &StringInfo;

	// used for strings
	_24AA32A_SendByte(Memory_String_Number,0x01);
	_24AA32A_SendByte(0x0002,0x01);  // write 256
	_24AA32A_SendByte(0x0003,0x00); 
		
	
	for (int i=0; i < 3; i++){
	WriteString2Memory(Douglass[i],0); /// should be in 256 chars or less
	printf("\n %s", Douglass[i]);
	printf("\n Wrote to device"); 
	}
	
  return; 
}


void WipeMemory(int startmem, int endmem){
	for (;startmem < endmem; startmem++){
		_24AA32A_SendByte(startmem,0x00);
		DataInDelay();
	}		

}

/*
// First 10k reserved for strings and string information
// first 2 bytes holds the number of strings stored, next 4 bytes there after hold Location& length for 400 bytes- 9600 bytes for strings
// Font, and serial store
/// String_Type   4096 bytes apparently
	1 = Quotes -> 0x0000 - 0x09FF-  2559 chars 
	2 =  Com Strings -> 0x0A00- 0x1000 - 1536 chars  
	
	
	//// Skim through the code remove the bloat 
*/
void WriteString2Memory(char *stringin, char String_Type){
	struct Memory_Application_Info *Mem_Var = &StringInfo;
		
	Mem_Var->String_Location =0;
	Mem_Var->String_Length= 0; 
	Mem_Var->String_Lenght_Count =0;
	 
	if (String_Type == 1){
		Mem_Var->Memory_Message_Number = Memory_Com_String_Number; 
		Mem_Var->Memory_Message_Section = Memory_Com_String_Section; 
		Mem_Var->Memory_Message_Location_Begin = Memory_Com_Location_Begin;
		Mem_Var->Memory_Message_Section_End = Memory_Com_String_End;
	}
	else{
		Mem_Var->Memory_Message_Number = Memory_String_Number;
		Mem_Var->Memory_Message_Section = Memory_String_Section;
		Mem_Var->Memory_Message_Location_Begin = Memory_String_Location_Begin;
		Mem_Var->Memory_Message_Section_End = Memory_String_End;
	}
	
	Mem_Var->String_Number = _24AA32A_ReadByte(Mem_Var->Memory_Message_Number);    // returns the number of strings in the memory module
	

	while(*stringin){
		Mem_Var->String_Lenght_Count++;
		stringin++;
	}
	
	stringin -= Mem_Var->String_Lenght_Count;
	/// read string location, this takes up to 2bytes of memory
	Mem_Var->String_Location = _24AA32A_ReadByte((((Mem_Var->String_Number)-1)*4)+Mem_Var->Memory_Message_Location_Begin); // read last 8 bits
	Mem_Var->String_Location <<= 8;
	Mem_Var->String_Location += _24AA32A_ReadByte(((((Mem_Var->String_Number)-1)*4) + Mem_Var->Memory_Message_Location_Begin)+1);  // read first 8 bits
	
	// If the string + string location is beyond the string boundries return and do nothing. 
	if ((Mem_Var->String_Lenght_Count + Mem_Var->String_Location) > Mem_Var->Memory_Message_Section_End) return; 



	for(int Send_Char=0; Send_Char < Mem_Var->String_Lenght_Count; Send_Char++){
		_24AA32A_SendByte(Mem_Var->String_Location, stringin[Send_Char]);
		Mem_Var->String_Location++;
		Mem_Var->String_Length++;
	}
	
	Mem_Var->String_Location+=1;
	// Send String Location 2 bytes worth
	_24AA32A_SendByte( ((((Mem_Var->String_Number)*4)+2) + Mem_Var->Memory_Message_Number), ((Mem_Var->String_Location >> 8) &0x00FF)  ); // send last 8 bits 0x0000----
	_24AA32A_SendByte( ((((Mem_Var->String_Number)*4)+3)+ Mem_Var->Memory_Message_Number) , (Mem_Var->String_Location & 0x00FF));  // send first 8 bits  0x----0000
	// Send String Length
	_24AA32A_SendByte( (((Mem_Var->String_Number*4)+0)+ Mem_Var->Memory_Message_Number) , ((Mem_Var->String_Lenght_Count >> 8) & 0x00FF) ); // send last 8 bits
	_24AA32A_SendByte( (((Mem_Var->String_Number*4)+1)+ Mem_Var->Memory_Message_Number) , Mem_Var->String_Lenght_Count);
	
	//Send String Number increase
	Mem_Var->String_Number++;
	_24AA32A_SendByte(Mem_Var->Memory_Message_Number,Mem_Var->String_Number);
	
}


////////////// NEEDS WORK!!!
// reads up to 256 chars 
char *ReadStringMemory(char stringnumber, char String_Type){
	struct Memory_Application_Info *Mem_Var = &StringInfo;
	/// wipe before using every time
	
	for (int x =0; x < 256; x++){ Mem_Var->Read_String[x]= 0x00;}
	
	if (String_Type == 1){
		Mem_Var->Memory_Message_Location_Begin= Memory_Com_String_Number;
	}
	else{
		Mem_Var->Memory_Message_Location_Begin= Memory_String_Number;
	}
	
	// Reading Location
	Mem_Var->String_Location = (char)_24AA32A_ReadByte(Mem_Var->Memory_Message_Location_Begin+((stringnumber*4)-2));
	Mem_Var->String_Location <<= 8;
	Mem_Var->String_Location += (char)_24AA32A_ReadByte(Mem_Var->Memory_Message_Location_Begin+((stringnumber*4)-1));

	// Reading String Length
	Mem_Var->String_Length = (char)_24AA32A_ReadByte(Mem_Var->Memory_Message_Location_Begin+ (stringnumber*4));
	Mem_Var->String_Length <<= 8;
	Mem_Var->String_Length += (char)_24AA32A_ReadByte(Mem_Var->Memory_Message_Location_Begin+ ((stringnumber*4)+1));

	//Store String in a central location for reading.
	for (Mem_Var->String_Inc= 0; Mem_Var->String_Inc < Mem_Var->String_Length ; Mem_Var->String_Inc++){
		Mem_Var->Read_String[Mem_Var->String_Inc] = _24AA32A_ReadByte((Mem_Var->String_Inc)+(Mem_Var->String_Location));
	}
	Mem_Var->Read_String[((Mem_Var->String_Location)+1)]= '\0';

	return Mem_Var->Read_String;
}


/*
void Memtest(void){
	struct Memory_Application_Info *Mem_Var = &StringInfo;
	HT1632_WriteString(0,2,2,0,"Done Wiping Mem");
	_24AA32A_SendByte(Memory_String_Number,0x01);
	_24AA32A_SendByte(Memory_String_Number,0x01);
	_24AA32A_SendByte(0x0002,0x01);  // write 256
	_24AA32A_SendByte(0x0003,0x00);
	
	HT1632_WriteString(0,2,2,0,"Writing Strings");
	for (int i=0; i < 6; i++){
		WriteString2Memory(Douglass[i]); /// should be in 256 + 15
		
	}

	HT1632_WriteString(0,2,2,0,"Reading Strings");
	HT1632_WriteString(0,2,2,0,ReadStringMemory(3));  //
	HT1632_WriteString(0,2,2,0,ReadStringMemory(2));  //
	HT1632_WriteString(0,2,2,0,ReadStringMemory(1));  // you need to wipe string_Return everytime
	// you use it

}



void StoreFont(void){
	char storebyte= 0x00;
	USART_putstring("reading ");
	for (int storeall=0; storeall < 94; storeall++)
	{
		for (char x=0; x< 5; x++)
		{
			storebyte = pgm_read_byte(font+(((storeall)*5)+x)); //
			_24AA32A_SendByte((Memory_Font_Start+x+(5*storeall)),storebyte);
			
		}
	}
	USART_putstring("reading ");
}

char *ReadFont(int font_value){
	char byte;
	_24AA32A_ReadByte(Memory_Font_Start+font_value);
	return byte;
}


*/ 