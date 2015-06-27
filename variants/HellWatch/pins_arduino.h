/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

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

*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define HELL_WATCH			1

#undef TCC2 //UNUSE
#undef TCD2 //UNUSE
#undef USARTC0_RXC_vect //UNUSE
#undef USARTC1_RXC_vect //UNUSE
#undef USARTD0_RXC_vect //UNUSE
#undef USARTD1_RXC_vect //UNUSE

#define Serial Serial4	//map Serial to Serial4

#define Wire xmWireC
#define Wire1 xmWireE

#define REPEAT8(x) x, x, x, x, x, x, x, x
#define BV0TO7 _BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7)
#define BV7TO0 _BV(7), _BV(6), _BV(5), _BV(4), _BV(3), _BV(2), _BV(1), _BV(0)

#define EXTERNAL_NUM_INTERRUPTS     30 // PORTA to PORTE 3

static const uint8_t SS	= 25;//ROM (SPI Flash)
static const uint8_t MOSI  	= 13;
static const uint8_t MISO  	= 14;
static const uint8_t SCK   	= 15;

static const uint8_t SDA 	= 8;
static const uint8_t SCL 	= 9;

#define OLED_DC 			18
#define OLED_CS 			16
#define OLED_RESET 		20		

#define SPI_PORT			SPIC

#ifdef ARDUINO_MAIN

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &PORTA,// 1
	(uint16_t) &PORTB,// 2
	(uint16_t) &PORTC,// 3
	(uint16_t) &PORTD,// 4
	(uint16_t) &PORTE,// 5
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
		// PORTLIST
		PE,/*  0 : PE0 : EXT SDA		*/
		PE,/*  1 : PE1 : EXT SCL/XCK0	*/
		PE,/*  2 : PE2 : EXT RXD0		*/
		PE,/*  3 : PE3 : EXT TXD0		*/
		PB,/*  4 : PB3 : EXT DAC1		*/
		PB,/*  5 : PB2 : EXT DAC0		*/
		PB,/*  6 : PB1 : EXT ADC9		*/
		PB,/*  7 : PB0 : EXT ADC8		*/
		PC,/*  8 : PC0 : SDA			*/
		PC,/*  9 : PC1 : SCL			*/
		PC,/* 10 : PC2 : NRF_IRQ		*/
		PC,/* 11 : PC3 : NRF_CE		*/
		PC,/* 12 : PC4 : NRF_CSN		*/
		PC,/* 13 : PC5 : MOSI			*/
		PC,/* 14 : PC6 : MISO			*/
		PC,/* 15 : PC7 : SCK			*/
		PD,/* 16 : PD0 : OLED_CS		*/
		PD,/* 17 : PD1 : OLED_SCLK		*/
		PD,/* 18 : PD2 : OLED_D/C		*/
		PD,/* 19 : PD3 : OLED_SDIN		*/
		PD,/* 20 : PD4 : OLED_RST		*/
		PD,/* 21 : PD5 : BEEPER : 1=ON, 0:OFF				*/
		PA,/* 22 : PA0 : BAT_ADC		*/
		PA,/* 23 : PA1 : KEY_ADC		*/
		PA,/* 24 : PA2 : ADC_EN		*/
		PA,/* 25 : PA3 : ROM_CS		*/
		PA,/* 26 : PA4 : VIBATOR : 1=ON, 0:OFF				*/
		PA,/* 27 : PA5 : RTC_ACC_INT						*/
		PA,/* 28 : PA6 : PWR_LOCK: 1=Power ON, 0: Power OFF	*/
		PA,/* 29 : PA7 : KEY_MAIN: 1=Key Down, 0: Key Up		*/
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
		// PIN IN PORT		
		// -------------------------------------------		
		_BV( 0 ),/*  0 : PE0 */
		_BV( 1 ),/*  1 : PE1 */
		_BV( 2 ),/*  2 : PE2 */
		_BV( 3 ),/*  3 : PE3 */
		_BV( 3 ),/*  4 : PB3 */
		_BV( 2 ),/*  5 : PB2 */
		_BV( 1 ),/*  6 : PB1 */
		_BV( 0 ),/*  7 : PB0 */
		_BV( 0 ),/*  8 : PC0 */
		_BV( 1 ),/*  9 : PC1 */
		_BV( 2 ),/* 10 : PC2 */
		_BV( 3 ),/* 11 : PC3 */
		_BV( 4 ),/* 12 : PC4 */
		_BV( 5 ),/* 13 : PC5 */
		_BV( 6 ),/* 14 : PC6 */
		_BV( 7 ),/* 15 : PC7 */
		_BV( 0 ),/* 16 : PD0 */
		_BV( 1 ),/* 17 : PD1 */
		_BV( 2 ),/* 18 : PD2 */
		_BV( 3 ),/* 19 : PD3 */
		_BV( 4 ),/* 20 : PD4 */
		_BV( 5 ),/* 21 : PD5 */
		_BV( 0 ),/* 22 : PA0 */
		_BV( 1 ),/* 23 : PA1 */
		_BV( 2 ),/* 24 : PA2 */
		_BV( 3 ),/* 25 : PA3 */
		_BV( 4 ),/* 26 : PA4 */
		_BV( 5 ),/* 27 : PA5 */
		_BV( 6 ),/* 28 : PA6 */
		_BV( 7 ),/* 29 : PA7 */
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	// TIMERS		
	// -------------------------------------------		
	/*  0 : PE0 */TIMER_E0A	,
	/*  1 : PE1 */TIMER_E0B	,
	/*  2 : PE2 */TIMER_E0C	,
	/*  3 : PE3 */TIMER_E0D	,
	/*  4 : PB3 */NOT_ON_TIMER,
	/*  5 : PB2 */NOT_ON_TIMER,
	/*  6 : PB1 */NOT_ON_TIMER,
	/*  7 : PB0 */NOT_ON_TIMER,
	/*  8 : PC0 */TIMER_C0A	,
	/*  9 : PC1 */TIMER_C0B	,
	/* 10 : PC2 */TIMER_C0C	,
	/* 11 : PC3 */TIMER_C0D	,
	/* 12 : PC4 */TIMER_C1A	,
	/* 13 : PC5 */TIMER_C1B	,
	/* 14 : PC6 */NOT_ON_TIMER,
	/* 15 : PC7 */NOT_ON_TIMER,
	/* 16 : PD0 */TIMER_D0A	,
	/* 17 : PD1 */TIMER_D0B	,
	/* 18 : PD2 */TIMER_D0C	,
	/* 19 : PD3 */TIMER_D0D	,
	/* 20 : PD4 */TIMER_D1A	,
	/* 21 : PD5 */TIMER_D1B	,
	/* 22 : PA0 */NOT_ON_TIMER,
	/* 23 : PA1 */NOT_ON_TIMER,
	/* 24 : PA2 */NOT_ON_TIMER,
	/* 25 : PA3 */NOT_ON_TIMER,
	/* 26 : PA4 */NOT_ON_TIMER, 
	/* 27 : PA5 */NOT_ON_TIMER, 
	/* 28 : PA6 */NOT_ON_TIMER,
	/* 29 : PA7 */NOT_ON_TIMER,
};
const TC0_t* const PROGMEM timer_to_tc0_PGM[] = {
/*  0 :  */	NULL,
/*  1 :  */	&TCC0,
/*  2 :  */	&TCC0,
/*  3 :  */	&TCC0,
/*  4 :  */	&TCC0,
/*  5 :  */	NULL,
/*  6 :  */	NULL,
/*  7 :  */	&TCD0,//NULL,
/*  8 :  */	&TCD0,//&TCC0,
/*  9 :  */	&TCD0,//&TCC0,
/* 10 :  */	&TCD0,//&TCC0,
/* 11 :  */	NULL,
/* 12 :  */	NULL,
/* 13 :  */	&TCE0,//NULL,
/* 14 :  */	&TCE0,//NULL,
/* 15 :  */	&TCE0,//NULL,
/* 16 :  */	&TCE0,//&TCD0,
};

const TC1_t* const PROGMEM timer_to_tc1_PGM[] = {
	/*	0 :  */ NULL,
	/*	1 :  */ NULL,
	/*	2 :  */ NULL,
	/*	3 :  */ NULL,
	/*	4 :  */ NULL,
	/*	5 :  */ &TCC1,
	/*	6 :  */ &TCC1,
	/*	7 :  */ NULL,
	/*	8 :  */ NULL,
	/*	9 :  */ NULL,
	/*  10 :  */ NULL,
	/*  11 :  */ &TCD1,
	/*  12 :  */ &TCD1,
	/*  13 :  */ &TCC1,
	/*  14 :  */ NULL,
	/*  15 :  */ NULL,
	/*  16 :  */ NULL,
};

const uint8_t PROGMEM timer_to_channel_PGM[] = {
	NOT_ON_TIMER,/*	0 :  */ 
    0,/*  1 :  */ 
    1,/*  2 :  */ 
    2,/*	  3 :  */ 
    3,/*	  4 :  */ 
    0,/*	  5 :  */ 
    1,/*  6 :  */ 
    2,/*  7 :  */ 
    3,/*  8 :  */ 
    0,/*	  9 :  */ 
    1,/*  10 :  */
    0,/*  11 :  */
    1,/*  12 :  */
    2,/*  13 :  */
    3,/*  14 :  */
    0,/*  15 :  */
    1,/*  16 :  */
};

const uint8_t PROGMEM adc_to_channel_PGM[] = {
    0,
    1,
    2,
    3,
    4,
    5,
	6,
	7,
	8,
	9,	
};

#endif

#endif
