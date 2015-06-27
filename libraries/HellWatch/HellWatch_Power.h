/*********************************************************************
This is a library for Hell Watch power manage

  ------> http://www.hellprototypes.com/

BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution

*********************************************************************/

#include "Arduino.h"


class HellWatch_Power {
 public:
	void begin(void);
	void systemPowerOff(void);
	uint16_t getBatteryVoltage(void);//unit: mV
	void analogSupplyCtrl(uint8_t ctrl);//Control OLED and analog(key, battery vol) supply
	void beep(uint16_t time);

 protected:

};