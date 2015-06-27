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

void HellWatch_Power::begin(void)
{
	//ADC
	ADCA.CH0.MUXCTRL  = 0x00;	// Channel 0 input: ADC0 pin
	ADCA.CH0.CTRL	  = 0x01;	// Single-ended positive input signal

	PORTA.PIN0CTRL	  = 0x07;  //Digital input buffer disabled

	//Buzzer
	uint16_t tone = 500;

	TCD1.PER = tone * 2;
	TCD1.CCB = (tone * 2) - (tone / 8);
	//TCD1.CTRLC = 0x02;//Match cmp when timer turn off ( Turn off buzzing MOS)
	TCD1.CTRLB = 0x23;//CCBEN, Single-slope PWM
}

void HellWatch_Power::systemPowerOff(void)
{
	PORTD.OUTCLR = 1 << 4;//Rest OLED
	analogSupplyCtrl(0);
	PORTA.OUTCLR = 1 << 6;//LDO lock disable
	while(1);//wait loop for key main release
}

void HellWatch_Power::analogSupplyCtrl(uint8_t ctrl)
{
	if(ctrl) {
		PORTA.OUTSET = 1 <<  2;
	} else {
		PORTA.OUTCLR = 1 <<  2;
	}
}

uint16_t HellWatch_Power::getBatteryVoltage(void)
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

void HellWatch_Power::beep(uint16_t time)
{
	TCD1.CTRLA = 0x04;//DIV8, 2MHz

	delay(time);

	while(!(PORTD.IN & (1 << 5)));//waiting for pin high
	while(PORTD.IN & (1 << 5));   //waiting for pin low
	TCD1.CTRLA = 0x00;//OFF
}

