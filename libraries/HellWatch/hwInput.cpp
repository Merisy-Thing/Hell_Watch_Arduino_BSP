/*********************************************************************
This is a library for Hell Watch Switch

  ------> http://www.hellprototypes.com/

BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution

*********************************************************************/
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>

#include "hwInput.h"


const uint8_t key_value_map[] = {
	255, 231, 210, 186, 158, 126, 86, 0,
};

#define START_ADC()			(ADCA.CH1.CTRL |= 1 << 7)
#define WAIT_CONVERT()		do{ while(!(ADCA.CH1.INTFLAGS & 0x01)); ADCA.CH1.INTFLAGS = 0x01;}while(0)

void Input::begin(void)
{
	//ADCA was inited by bootloader
	ADCA.CTRLA		  = 0x01;	// Enable ADC
}

uint8_t Input::getKey(void)
{
	uint8_t  adc0, key;

    key = SW_NONE;
	if(PORTA.IN & (1 << 7)) {
		key = SW_MAIN;
	} else	{
		START_ADC();
		WAIT_CONVERT();
		adc0 = ADCA.CH1.RESL;
		if(adc0 <= 250) {
			delay(20);
			START_ADC();
			WAIT_CONVERT();
            if(adc0 - ADCA.CH1.RESL < 2) {
				for(key = 0; key < 7; key++) {
					if(adc0 >= key_value_map[key]) {
                        delay(20);
						break;
					}
				}
            }
		}
	}

	return key;
}

void Input::waitKeyUp(void)
{
    //FIXME: Add timeout return
    while(getKey() != SW_NONE) {
		delay(10);
	}
}
