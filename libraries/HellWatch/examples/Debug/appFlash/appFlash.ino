#include <SPI.h>
#include <HellWatch.h>

HellWatch hell;

uint32_t addr = (uint32_t)768 * (uint32_t)1024;

void setup(){
	Serial.begin(115200);
	hell.input.begin();

	hell.setCursor(0, 0);
	hell.print("MF:0x");
	hell.println(hell.appFlash.readManufacturer(), HEX);
	hell.display();
}

void loop(){
	uint8_t buf;
	uint8_t sw = hell.input.getInput();
	if(sw == SW_3WS_UP) {
		addr++;
	} else if(sw == SW_3WS_DOWN) {
		addr--;
	} else {
		delay(10);
		return;
	}
	delay(50);

	hell.appFlash.read(addr, &buf, 1);

	hell.clearDisplay();
	hell.setCursor(0, 8);
	hell.print(addr, HEX);
	hell.print(": ");
	hell.print(buf, HEX);
	hell.display();
}
