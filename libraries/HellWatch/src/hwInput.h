/*********************************************************************
This is a library for Hell Watch Button

  ------> http://www.hellprototypes.com/

BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution

*********************************************************************/
#ifndef _HW_INPUT_H
#define _HW_INPUT_H

#include "Arduino.h"

#define SW_NONE					0
#define SW_BTM_LEFT				1
#define SW_BTM_RIGHT			2
#define SW_3WS_DOWN				3
#define SW_3WS_PUSH				4
#define SW_3WS_UP				5
#define SW_TOP_RIGHT			6
#define SW_TOP_LEFT				7
#define SW_MAIN					8
#define SW_WAIT_FLAG			0xFF

class Input {
 public:
	void begin(void);

	uint8_t getInput(void);
    void waitUp(void);

 protected:

};

#endif