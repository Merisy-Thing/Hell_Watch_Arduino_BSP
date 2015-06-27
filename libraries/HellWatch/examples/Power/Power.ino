#include <SPI.h>
#include <HellWatch.h>

HellWatch hell;

void setup(){
	Serial.begin(115200);
	hell.input.begin();
	hell.power.begin();

	hell.setCursor(8, 16);
	hell.print(hell.power.getBatteryVoltage());
	hell.println("mV");
	hell.display();
}

void loop(){
	uint8_t key = hell.input.getKey();
	if(key == SW_3WS_PUSH) {
		hell.power.turnOff();
	}
	delay(10);
}
