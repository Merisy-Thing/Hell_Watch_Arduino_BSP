/******************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers
  
This example is for a 128x64 size display using SPI to communicate
4 or 5 pins are required to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution


--11/20/2012--
Modified to use hardware SPI on both Arduino Uno and Akafuino X by Nathan Duprey
Only for 128x64 SSD1306 OLED Displays
Pins Map:
	-Display Data to Arduino digital 11
	-Display CLK to Arduino digital 13
	-Display DC, Rst, and CS can be any pins
************************************************************************/
#include <Adafruit_GFX.h>
#include <Xmega_SSD1306.h>


Xmega_SSD1306 display(OLED_CS, OLED_RESET, OLED_DC);

void setup(){
  display.begin(SSD1306_SWITCHCAPVCC);
  
  display.clearDisplay();
  display.display();
}

void loop(){
  display.setTextColor(WHITE);
  
  for(float i = 00.00; i < 15.00; i += 00.01){
    display.setCursor(0, 0);
    
    if(i < 10.00){
      display.setTextSize(1);
      display.print(0);
      display.println(i);
      
      display.setTextSize(2);
      display.print(0);
      display.println(i);
      
      display.setTextSize(3);
      display.print(0);
      display.println(i);
    }else{
      display.setTextSize(1);
      display.println(i);
      
      display.setTextSize(2);
      display.println(i);
      
      display.setTextSize(3);
      display.println(i);
    }
    display.display();
    display.clearDisplay();
  }
}
