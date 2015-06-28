/*********************************************************************
This is a library for Hell Watch power manage

  ------> http://www.hellprototypes.com/

BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution

*********************************************************************/
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>

#include "hwPower.h"

void Power::begin(void)
{
	//ADC
	ADCA.CH0.MUXCTRL  = 0x00;	// Channel 0 input: ADC0 pin
	ADCA.CH0.CTRL	  = 0x01;	// Single-ended positive input signal

	PORTA.PIN0CTRL	  = 0x07;  //Digital input buffer disabled

	ADCA.CTRLA		  = 0x01;	// Enable ADC
}

void Power::turnOff(void)
{
	PORTD.OUTCLR = 1 << 4;//Rest OLED
	delay(10);
	PORTA.OUTCLR = 1 << 2;//OLED power off
	delay(10);
	PORTA.OUTCLR = 1 << 6;//LDO lock disable
	while(1);//wait loop for key main release
}

uint16_t Power::getBatteryVoltage(void)
{
    uint16_t voltage;

	byte old_ref = ADCA.REFCTRL;

	ADCA.REFCTRL  = 0x02;	// REF = 1V

	ADCA.CH0.CTRL |= 1 << 7;

	while(!(ADCA.CH0.INTFLAGS & (1 << 0)));
	ADCA.CH0.INTFLAGS = 0x01;
    voltage = ((unsigned long)ADCA.CH0.RESL * 4900) / 256 - 245;//0.05*4900 = 245

	ADCA.REFCTRL = old_ref;

    return voltage;
}

