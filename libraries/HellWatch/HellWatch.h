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

class HellWatch : public Display
{
 public:

	Input input;
	Power power;
 protected:

};

#endif