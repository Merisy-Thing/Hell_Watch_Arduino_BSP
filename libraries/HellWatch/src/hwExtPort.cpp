/*********************************************************************
This is a library for Hell Watch Ext interface

  ------> http://www.hellprototypes.com/

BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution

*********************************************************************/

#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>

#include "hwExtPort.h"

#define START_ADC_CH3()	(ADCA.CH3.CTRL |= 1 << 7)
#define WAIT_CONVERT3()	do{ while(!(ADCA.CH3.INTFLAGS & 0x01)); ADCA.CH3.INTFLAGS = 0x01;}while(0)

void ExtPort::AdcBegin(uint8_t pin)//Pin7(PB0):ADC8, Pin6(PB1):ADC9, Pin5(PB2):ADC10, Pin4(PB3):ADC11
{
	if(pin == 7) {
		PORTB.DIRCLR = 1 << 0;
		PORTB.PIN0CTRL	  = 0x07;  //Digital input buffer disabled
	} else if(pin == 6) {
		PORTB.DIRCLR = 1 << 1;
		PORTB.PIN1CTRL	  = 0x07;
	} else if(pin == 5) {
		PORTB.DIRCLR = 1 << 2;
		PORTB.PIN2CTRL	  = 0x07;
	} else if(pin == 4) {
		PORTB.DIRCLR = 1 << 3;
		PORTB.PIN3CTRL	  = 0x07;
	} else {
		return;
	}

	//MAKE SURE: Same as button adc config
	ADCA.CTRLB 		= 0x64;
	ADCA.PRESCALER  = 0x07;
	ADCA.CTRLA		= 0x01;
	ADCA.REFCTRL	= 0x12;

	ADCA.CH3.CTRL	= 0x01;
}
uint8_t ExtPort::AdcRead8(uint8_t pin)
{
	uint8_t adc_val;
	uint8_t old_refctrl = ADCA.REFCTRL;
	
	ADCA.REFCTRL	  = 0x12;	// REF= VCC/1.6 (3.3V/1.6 = 2.0625V)
	ADCA.CH3.MUXCTRL  = (7 - pin + 8) << 3;	// Channel  input set
	START_ADC_CH3();
	WAIT_CONVERT3();

	adc_val = ADCA.CH3.RESL;

	ADCA.REFCTRL = old_refctrl;

	return adc_val;
}

uint16_t ExtPort::AdcRead12(uint8_t pin)
{
	uint16_t adc_val;
	uint8_t old_refctrl = ADCA.REFCTRL;
	uint8_t old_ctrlb = ADCA.CTRLB;

	ADCA.CTRLB &= 0xF8; //12BIT
	ADCA.REFCTRL	  = 0x12;
	ADCA.CH3.MUXCTRL  = (7 - pin + 8) << 3;

	START_ADC_CH3();
	WAIT_CONVERT3();

	adc_val = ADCA.CH3.RES;

	ADCA.REFCTRL = old_refctrl;
	ADCA.CTRLB = old_ctrlb;

	return adc_val;
}

void ExtPort::AdcEnd(uint8_t pin)
{
	if(pin == 7) {
		PORTB.PIN0CTRL	  = 0x00;  //Digital input buffer disabled
	} else if(pin == 6) {
		PORTB.PIN1CTRL	  = 0x00;
	} else if(pin == 5) {
		PORTB.PIN2CTRL	  = 0x00;
	} else if(pin == 4) {
		PORTB.PIN3CTRL	  = 0x00;
	}
	ADCA.CH3.MUXCTRL  = 0x00;	// Channel 1 input: ADC1 pin
	ADCA.CH3.CTRL	  = 0x00;	// Single-ended positive input signal
}
//====================================================================
//TODO
void ExtPort::DacBegin(uint8_t pin)//Pin4:DAC1, Pin5:DAC0
{

}
void ExtPort::DacWrite(uint8_t pin, uint16_t data)
{

}
void ExtPort::DacEnd(uint8_t pin)
{

}
//====================================================================
//Pin0~3: PE0~3, Pin4~7: PB3~0
static const uint8_t PROGMEM swap[] {
	0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
};
uint8_t ExtPort::h_bits_swap(uint8_t data)
{
	return (data & 0x0F) | pgm_read_byte(swap + (data >> 4));
}
void ExtPort::PortBegin(uint8_t dir_mask)
{
	dir_mask = h_bits_swap(dir_mask);
	PORTE.DIR = dir_mask & 0x0F;
	PORTB.DIR = dir_mask >> 4;
}
void ExtPort::PortPullUp(uint8_t mask)
{
	mask = h_bits_swap(mask);
	if((mask & 0x0F) > 0) {
		PORTCFG.MPCMASK = mask & 0x0F;
		PORTE.PIN0CTRL = 0x18;
	}
	if((mask & 0xF0) > 0) {
		PORTCFG.MPCMASK = mask >> 4;
		PORTB.PIN0CTRL = 0x18;
	}
}
void ExtPort::PortPullDown(uint8_t mask)
{
	mask = h_bits_swap(mask);
	if((mask & 0x0F) > 0) {
		PORTCFG.MPCMASK = mask & 0x0F;
		PORTE.PIN0CTRL = 0x10;
	}
	if((mask & 0xF0) > 0) {
		PORTCFG.MPCMASK = mask >> 4;
		PORTB.PIN0CTRL = 0x10;
	}
}
uint8_t ExtPort::PortRead(void)
{
	return 	h_bits_swap( ((PORTB.IN & 0x0F) << 4) | (PORTE.IN & 0x0F) );
}
void ExtPort::PortWrite(uint8_t data)
{
	uint8_t de, db;
	data = h_bits_swap(data);
	de = data & 0x0F;
	db = data >> 4;
	PORTE.OUT = de;
	PORTB.OUT = db;
}
void ExtPort::PortEnd(uint8_t mask)
{
	mask = h_bits_swap(mask);
	if((mask & 0x0F) > 0) {
		PORTE.DIRCLR = mask & 0x0F;
		PORTCFG.MPCMASK = mask & 0x0F;
		PORTE.PIN0CTRL = 0x00;
	}
	if((mask & 0xF0) > 0) {
		PORTB.DIRCLR = 	 mask >> 4;;
		PORTCFG.MPCMASK = mask >> 4;
		PORTB.PIN0CTRL = 0x00;
	}
}

