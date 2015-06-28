/*********************************************************************
Library for Hell Watch

  ------> http://www.hellprototypes.com/

BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution

*********************************************************************/
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>

#include "HellWatch.h"

HellWatch::HellWatch()
{
	flash.begin();
}

void HellWatch::flashRead(uint32_t addr, uint8_t *buf, uint16_t n)
{
	flash.read(addr, buf, n);
}
