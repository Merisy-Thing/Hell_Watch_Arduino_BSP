/*
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.c 808 2009-12-18 17:44:08Z dmellis $
*/

#include <stddef.h>
#include <avr/pgmspace.h>
#include "wiring_private.h"

volatile unsigned long milliseconds = 0;

ISR(TCE0_OVF_vect)
{
	milliseconds++;
}

unsigned long millis()
{
 	unsigned long m;
 	
 	uint8_t oldSREG = SREG;
 
	// disable interrupts while we read rtc_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to rtc_millis)
	cli();
	m = milliseconds;
	SREG = oldSREG;

	return m;
}

unsigned long micros(void) {

	unsigned long m;
	uint16_t u;

    uint8_t oldSREG = SREG;

    // disable interrupts while we read rtc_millis or we might get an
    // inconsistent value (e.g. in the middle of a write to rtc_millis)
    cli();
    m = milliseconds;
	u = TCE0.CNT;
    SREG = oldSREG;

    return m*1000 + (u >> 1);
}

/* Delay for the given number of microseconds.  Assumes a 8, 16 or 32 MHz clock. */
void delayMicroseconds(unsigned int us)
{
	// for the 16 MHz clock on hell watch boards

	// for a one-microsecond delay, simply return.  the overhead
	// of the function call yields a delay of approximately 1 1/8 us.
	if (--us == 0)
			return;

	// the following loop takes a quarter of a microsecond (4 cycles)
	// per iteration, so execute it four times for each microsecond of
	// delay requested.
	us <<= 2;

	// account for the time taken in the preceeding commands.
	us -= 2;

	// busy wait
	__asm__ __volatile__ (
			"1: sbiw %0,1" "\n\t" // 2 cycles
			"brne 1b" : "=w" (us) : "0" (us) // 2 cycles
	);
}


void delay(unsigned long ms)
{
	unsigned long start = millis();
	
	while (millis() - start <= ms);
}

void init()
{
	//Use TCE0 for millis etc.(0.5uS)
 	TCE0.PER = 1999;
 	TCE0.INTCTRLA = 0x01; //LO Pri
 	TCE0.CTRLA = 0x04;//DIV 8,  2M

	/* Enable interrupts.                */
	PMIC.CTRL |= PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
	sei();
}
