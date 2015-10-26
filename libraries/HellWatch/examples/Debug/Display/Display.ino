#include <SPI.h>
#include <HellWatch.h>

HellWatch hell;

void setup(){
	Serial.begin(115200);
  
	hell.setCursor(8, 16);
	hell.print("Hello World!");
	hell.display();
}

void loop(){
	Serial.print("Hello World!");

	delay(1000);
}
