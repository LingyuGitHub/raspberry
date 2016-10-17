#include <ArduiPi_SSD1306.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <stdio.h>

#include "mcomn.h"
#include "ssd1306.h"

Adafruit_SSD1306 display;

extern bool oled_init(){
	if( !display.init(OLED_I2C_RESET,2) ){
		return false;
	}
	display.begin();
	display.setTextSize(2);
	display.setTextColor(WHITE);
	return true;
}

extern void oled_writetext(char* str){
	display.clearDisplay();
	display.setCursor(0,10);
	display.print(str);
	display.display();
}
