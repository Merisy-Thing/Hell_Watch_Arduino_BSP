/*********************************************************************
This is a library for Hell Watch Ext interface

  ------> http://www.hellprototypes.com/

BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution

*********************************************************************/

#include "Arduino.h"

class HellWatch_Ext {
 public:
	//Pin7(PB0):ADC8, Pin6(PB1):ADC9, Pin5(PB2):ADC10, Pin4(PB3):ADC11
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
