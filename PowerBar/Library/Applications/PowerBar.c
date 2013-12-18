/*
 * PowerBar.c
 *
 * Created: 7/3/2013 3:13:52 PM
 *  Author: mfolz
 */ 
/// client and server names will get confusing 

#include "PowerBar.h"

void PowerBar_Application(void){
	char com_state;
	char *data= "PONG"; 
	Server_Start(sizeof(data), "clie1");
	printf("\r Initialized Client: %s ", "clie1");
	printf("\r All devices will be off by default" );
	PowerBar_DeviceManagement(0x00);
	printf("\r Turn on any device using the server only or hitting one of the main switches");
	
	
	while(1){
		// Run routine that shuts off all devices 		
		Server_RecieveData("clie1", (char*)*&data);
		printf("\r Received: %s", data);
		data[0]= 'D';
		printf("\r Sending out: %s", data);
		Server_SendData("clie1", (char*)*&data);
	
		}
}


void Draw_Gui(void){
	LCD_Fill_Rectangle(0,0,128,160, 0x0000000);
	LCD_Write_Line("POWER BAR CONTROLER:",0,140, 2, 0xFFFFFFF);
	LCD_Draw_Line(0,40,160,40,7,0x0000FF);
	
	LCD_Write_Line("FRONT:", 5, 80, 2, 0xFFFFFF);
	LCD_Write_Line("BACK:", 5, 60, 2, 0xFFFFFF);
	LCD_Write_Line("COMMAND:", 0, 30, 2, 0xFFFFFF);
	
}




void IR_Routine(void){
	struct DS1307_Properties *point_time = &Time;
	int numberpulses;
	char time[5]; 
	
// 	LCD_Fill_Rectangle(0,0,120,30, 0x000000);
// 	DS1307_Copy_Date();
// 	LCD_Write_Line(point_time->Display_Date, 0,40,2,0x00FFFF);

// 		DS1307_ReadTime(4,0x00FFFF, DS_Hours); 
// 		DS1307_ReadTime(80, 0x00FFFF, DS_Minutes);
// 	
	numberpulses = IR_Detect(); // dumb code just sits in a while loop 
		/// this needs to be interrupt driven! 
	
	if (numberpulses >= 32){
		if (IRcompare(numberpulses, Temp_Down_Button, NUMPULSES ))
		{
			CLEARBIT(POWERBAR_PORT, LIGHT_PIN);
			LCD_Fill_Rectangle(75,80,100,20, 0x000000);
			LCD_Fill_Rectangle(0,0,120,30, 0x000000);
			LCD_Write_Line("OFF", 90 , 80, 2, 0xFF0000);
			LCD_Write_Line("FRONT OFF", 10, 10, 2, 0xFF0000);
			
		}
		
		if (IRcompare(numberpulses, Temp_UP_Button, NUMPULSES ))
		{
			SETBIT(POWERBAR_PORT, LIGHT_PIN);
			LCD_Fill_Rectangle(75,80,100,20, 0x000000);
			LCD_Fill_Rectangle(0,0,120,30, 0x000000);
			LCD_Write_Line("ON", 90 , 80, 2, 0x00FF00);
			LCD_Write_Line("FRONT ON", 10, 10, 2, 0x00FF00);
		}
		
		if (IRcompare(numberpulses, Fire_Down_Button, NUMPULSES ) )
		{
			
			CLEARBIT(POWERBAR_PORT, FAN_PIN);
			LCD_Fill_Rectangle(75,60,100,20, 0x000000);
			LCD_Fill_Rectangle(0,0,120,30, 0x000000);
			LCD_Write_Line("OFF", 90 , 60, 2, 0xFF0000);
			LCD_Write_Line("BACK OFF", 10, 10, 2, 0xFF0000);
		}
		
		if (IRcompare(numberpulses, Fire_UP_Button, NUMPULSES ) )
		{
			
			SETBIT(POWERBAR_PORT, FAN_PIN);
			LCD_Fill_Rectangle(75,60,100,20, 0x000000);
			LCD_Fill_Rectangle(0,0,120,30, 0x000000);
			LCD_Write_Line("ON", 90 , 60, 2, 0x00FF00);
			LCD_Write_Line("BACK ON", 10, 10, 2, 0x00FF00);
		}
		
		if (IRcompare(numberpulses,All_ON, NUMPULSES)){
			SETBIT(POWERBAR_PORT, FAN_PIN);
			SETBIT(POWERBAR_PORT, LIGHT_PIN);
			LCD_Fill_Rectangle(75,80,100,20, 0x000000);
			LCD_Fill_Rectangle(75,60,100,20, 0x000000);
			LCD_Fill_Rectangle(0,0,120,30, 0x000000);
			LCD_Write_Line("ON", 90 , 80, 2, 0x00FF00);
			LCD_Write_Line("ON", 90 , 60, 2, 0x00FF00);
			LCD_Write_Line("ALL ON", 10, 10, 2, 0x00FF00);			
		}
		
			if (IRcompare(numberpulses,All_OFF, NUMPULSES)){
				CLEARBIT(POWERBAR_PORT, FAN_PIN);
				CLEARBIT(POWERBAR_PORT, LIGHT_PIN);
				LCD_Fill_Rectangle(75,80,100,20, 0x000000);
				LCD_Fill_Rectangle(75,60,100,20, 0x000000);
				LCD_Fill_Rectangle(0,0,120,30, 0x000000);
				LCD_Write_Line("OFF", 90 , 60, 2, 0xFF0000);
				LCD_Write_Line("OFF", 90 , 80, 2, 0xFF0000);
				LCD_Write_Line("ALL OFF", 10, 10, 2, 0xFF0000);
		}
		
	}
	
}






void PowerBar_DeviceManagement(char device){
	
	
}