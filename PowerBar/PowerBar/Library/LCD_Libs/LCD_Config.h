/*
 * LCD_Config.h
 *
 * Created: 7/11/2013 1:10:10 PM
 *  Author: mfolz
 */ 


#ifndef LCD_CONFIG_H_INCLUDED_
#define LCD_CONFIG_H_INCLUDED_
/*
LCD Driver:
---------------------------------------
	-Set the connected LCD to 1 leave all others 0 
*/ 

#define ST7735R 0
#define ST7735	1


/*
	SSD1289:
		-Can be programmed using either SPI or Parallel communications
		-Parallel is highly recommended as with SPI it can take almost 1 second to wipe
		a screen. 
*/

#define SSD1289 0
#define SSD1289_SPI 0
#define SSD1289_Parallel 0 

/*
Set Rotation:
---------------------------------------
	-Set the desired layout, landscape or portrait. 
	-Left or Right 
*/
#define Landscape 0 
#define Portrait 0 
#define Display_Left 0 
#define Display_Right 0 
	





//#define 


#endif /* LCD_CONFIG_H_ */