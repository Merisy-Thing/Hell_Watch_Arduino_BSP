/*********************************************************************
This is a library for Hell Watch Button

  ------> http://www.hellprototypes.com/

BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution

*********************************************************************/

#include "Arduino.h"

#define BTN_NONE				0
#define BTN_BTM_LEFT			1
#define BTN_BTM_RIGHT			2
#define BTN_3WS_DOWN			3
#define BTN_3WS_PUSH			4
#define BTN_3WS_UP				5
#define BTN_TOP_RIGHT			6
#define BTN_TOP_LEFT			7
#define BTN_MAIN				8
#define BTN_WAIT_FLAG			0xFF

class HellWatch_Button {
 public:
	void begin(void);
	uint8_t getButton(void);
    void waitButtonUp(void);
 protected:

};