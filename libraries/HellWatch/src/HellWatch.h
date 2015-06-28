/*********************************************************************
This is a library for Hell Watch power manage

  ------> http://www.hellprototypes.com/

BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution

*********************************************************************/
#ifndef _HELL_WATCH_h
#define _HELL_WATCH_h

#include <Arduino.h>
#include <avr/pgmspace.h>

#include <hwDisplay.h>
#include <hwInput.h>
#include <hwPower.h>
#include <hwExtPort.h>
#include <hwFlash.h>

class HellWatch : public Display
{
 public:
	HellWatch();

	Input input;
	Power power;
	ExtPort extPort;
	Flash flash;

	virtual void flashRead(uint32_t addr, uint8_t *buf, uint16_t n);
 protected:

};

#endif