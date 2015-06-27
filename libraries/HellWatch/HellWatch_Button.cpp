/*********************************************************************
This is a library for Hell Watch Button

  ------> http://www.hellprototypes.com/

BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution

*********************************************************************/
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>

#include "HellWatch_Button.h"


const uint8_t key_value_map[] = {
	255, 231, 210, 186, 158, 126, 86, 0,
};

#define START_ADC()		(ADCA.CH1.CTRL |= 1 << 7)
#define WAIT_CONVERT()		do{ while(!(ADCA.CH1.INTFLAGS & 0x01)); ADCA.CH1.INTFLAGS = 0x01;}while(0)

void HellWatch_Button::begin(void)
{
	//ADCA.CALL and ADCA.CALH was inited by bootloader

	ADCA.CTRLB = 0x64;			// High current limit, signed mode, no free run, 8 bit
	ADCA.PRESCALER = 0x07;		// Prescaler 512 (62.5kHZ ADC clock)
	ADCA.CTRLA		  = 0x01;	// Enable ADC

	ADCA.REFCTRL	  = 0x12;	// REF= VCC/1.6 (3.3V/1.6 = 2.0625V)
	ADCA.CH1.MUXCTRL  = 0x08;	// Channel 1 input: ADC1 pin
	ADCA.CH1.CTRL	  = 0x01;	// Single-ended positive input signal
}
uint8_t HellWatch_Button::getButton(void)
{
	uint8_t  adc0, key;

    key = BTN_NONE;
	if(PORTA.IN & (1 << 7)) {
		key = BTN_MAIN;
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

void HellWatch_Button::waitButtonUp(void)
{
    //FIXME: Add timeout return
    while(getButton() != BTN_NONE);
}
