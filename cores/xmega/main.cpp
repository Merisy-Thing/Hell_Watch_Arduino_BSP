#include <Arduino.h>

int main(void)
{
	init();

	setup();
    
	for (;;) {
		loop();
		if (serialEventRun) serialEventRun();

		//For Hell Watch: 
		//  Press main key to force power off
		//  DO NOT let program enter endless loop at setup function
		if(PORTA.IN & (1 << 7)) {
			PORTD.OUTCLR = 1 << 4;//Rest OLED
			PORTA.OUTCLR = 1 << 2;//OLED Power OFF
			PORTA.OUTCLR = 1 << 6;//LDO lock disable
			while(1);//wait loop for key main release
		}
	}
        
	return 0;
}

