#include <Arduino.h>

int main(void)
{
    uint16_t start;
    uint8_t  load_start = 0;

	init();

	setup();
    
	for (;;) {
		loop();
		if (serialEventRun) serialEventRun();

		//For Hell Watch: 
		//  Long press main key to force power off
		//  DO NOT let program enter endless loop at setup function
		if(PORTA.IN & (1 << 7)) {
            if(!load_start) {
                load_start = 1;
                start = (uint16_t)millis();
            } else {
                if((uint16_t)millis() - start > 3000 ) { //
					PORTD.OUTCLR = 1 << 4;//Rest OLED
					PORTA.OUTCLR = 1 << 2;//OLED Power OFF
					PORTA.OUTCLR = 1 << 6;//LDO lock disable
					while(1);//wait loop for key main release
				}
            }
		} else {
            load_start = 0;
        }
	}
        
	return 0;
}

