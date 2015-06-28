#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Xmega_SSD1306.h>
#include <HellWatch_Button.h>
#include <HellWatch_Power.h>
#include <HellWatch_Ext.h>
#include <Rtc_Pcf8563.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include "I2Cdev.h"
#include "MPU6050.h"

Xmega_SSD1306 display(OLED_CS, OLED_RESET, OLED_DC);
HellWatch_Power power;
HellWatch_Button button;
HellWatch_Ext external;

Rtc_Pcf8563 rtc;

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

void setup(){
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC);
  power.begin();
  button.begin();
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE);
  external.AdcBegin(7);
  external.AdcBegin(6);
  external.PortBegin(0x00);
  external.PortPullUp(0x3f);

  Wire.begin();
  rtc.initClock();
  //rtc.setDate(14, 6, 3, 0, 10);
  //hr, min, sec
  //rtc.setTime(1, 15, 40);
  accelgyro.initialize();
}

void loop(){
	delay(500);
	display.setCursor(0, 0);
	display.print("vol=");
	display.print(power.getBatteryVoltage());

	display.print("  btn=");
	uint8_t sw = button.getButton();
	display.println(sw);

	if(sw == BTN_BTM_RIGHT) {
		display.clearDisplay();
		display.print("Power OFF");
		display.display();
		delay(1000);
		power.systemPowerOff();
	}

	display.print(rtc.formatTime());
	display.print(" ");
	display.println(rtc.formatDate());

	//8Bits ADC, 11, 118, 235
	display.print("P6=");
	display.print(external.AdcRead8(6));
	display.print(" P7=");
	display.print(external.AdcRead8(7));
	display.print(" PT=");
	display.println(external.PortRead() & 0x3F);

	//12Bits ADC
	//display.print("P6=");
	//display.print(external.AdcRead12(6));
	//display.print(" P7=");
	//display.println(external.AdcRead12(7));

	accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

	display.print(ax); display.print(" ");
	display.print(ay); display.print(" ");
	display.println(az);
	display.print(gx); display.print(" ");
	display.print(gy); display.print(" ");
	display.println(gz);

	display.display();
	display.clearDisplay();
}
