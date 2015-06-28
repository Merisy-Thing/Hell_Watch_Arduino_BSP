/*********************************************************************
This is a library for Hell Watch Ext interface

  ------> http://www.hellprototypes.com/

BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution

*********************************************************************/
#ifndef _HW_EXTPORT_H
#define _HW_EXTPORT_H

#include "Arduino.h"

/* Pin 0 : PE0 : EXT SDA		*/
/* Pin 1 : PE1 : EXT SCL/XCK0	*/
/* Pin 2 : PE2 : EXT RXD0		*/
/* Pin 3 : PE3 : EXT TXD0		*/
/* Pin 4 : PB3 : EXT DAC1		*/
/* Pin 5 : PB2 : EXT DAC0		*/
/* Pin 6 : PB1 : EXT ADC9		*/
/* Pin 7 : PB0 : EXT ADC8		*/

class ExtPort {
 public:
	void AdcBegin(uint8_t pin);
	uint8_t AdcRead8(uint8_t pin);
	uint16_t AdcRead12(uint8_t pin);
	void AdcEnd(uint8_t pin);

	void DacBegin(uint8_t pin);//Pin4:DAC1, Pin5:DAC0
	void DacWrite(uint8_t pin, uint16_t data);
	void DacEnd(uint8_t pin);

    void PortBegin(uint8_t dir_mask);
	void PortPullUp(uint8_t mask);
	void PortPullDown(uint8_t mask);
	uint8_t PortRead(void);
	void PortWrite(uint8_t data);
	void PortEnd(uint8_t mask);

 protected:
	 uint8_t h_bits_swap(uint8_t data);
};
#endif