/*********************************************************************
This is a library for Hell Watch power manage

  ------> http://www.hellprototypes.com/

BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution

*********************************************************************/
#ifndef _HW_POWER_H
#define _HW_POWER_H

#include "Arduino.h"


class Power {
 public:
	void begin(void);

	void turnOff(void);
	uint16_t getBatteryVoltage(void);//unit: mV

 protected:

};

#endif