#include <SPI.h>
#include <HellWatch.h>

HellWatch hell;

void setup(){
	Serial.begin(115200);
	hell.input.begin();

	hell.setCursor(8, 16);
	hell.print("Hello World!");
	hell.display();
}

void loop(){
	uint8_t key = hell.input.getKey();
	if(key != SW_NONE) {
		hell.setCursor(16, 24);
		hell.print(key);
		hell.display();
	}
}
