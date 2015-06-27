/*
 * (C) Copyright 2014 Aur¨¦lien Rodot. All rights reserved.
 *
 * This file is part of the Gamebuino Library (http://gamebuino.com)
 *
 * The Gamebuino Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "Battery.h"

void Battery::begin() {
#if (ENABLE_BATTERY > 0)
#ifdef HELL_WATCH
	ADCA.CH0.MUXCTRL  = 0x00;	// Channel 0 input: ADC0 pin
	ADCA.CH0.CTRL	  = 0x01;	// Single-ended positive input signal

	PORTA.PIN0CTRL	  = 0x07;  //Digital input buffer disabled
#else
    voltage = BAT_LVL_FULL;
    level = NUM_LVL;
    nextUpdate = 1;
	//show = false;
#endif//#ifdef HELL_WATCH
#endif
}

void Battery::update() {
#if (ENABLE_BATTERY > 0)
    if (!(nextUpdate % 32)) { //every 32 frames
    #ifdef HELL_WATCH
		uint8_t old_ref = ADCA.REFCTRL;
		ADCA.REFCTRL  = 0x02;	// REF = 1V
		ADCA.CH0.CTRL |= 1 << 7;
		while(!(ADCA.CH0.INTFLAGS & (1 << 0)));
		ADCA.CH0.INTFLAGS = 0x01;
	    voltage = ((unsigned long)ADCA.CH0.RESL * 4900) / 256 - 245;//0.05*4900 = 245
		ADCA.REFCTRL = old_ref;
	#else
        voltage = analogRead(BAT_PIN)*6.4453; //3.3V * 2 *1000 / 1024
    #endif
		if(voltage){
        //set the battery 'level' according to thresholds
        level = NUM_LVL;
            for (uint8_t i = 0; i < NUM_LVL; i++) {
                if (voltage < thresolds[i]) {
                    level = i;
                    return;
                }
            }
	 	}
		else{
			level = 255;
		}
    }
    nextUpdate++;
#endif
}

