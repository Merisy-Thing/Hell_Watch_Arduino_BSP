#include <HellWatch.h>
#include <Wire.h>
#include <Rtc_Pcf8563.h>

Rtc_Pcf8563 rtc;
HellWatch hell;

void setup()
{
  //clear out all the registers
  rtc.initClock();
  //set a time to start with.
  //day, weekday, month, century, year
  rtc.setDate(14, 6, 3, 0, 15);
  //hr, min, sec
  rtc.setTime(1, 15, 40);
}

void loop()
{
  hell.clearDisplay();
  hell.setCursor(0, 0);
  hell.println(rtc.formatTime());
  hell.print(rtc.formatDate());
  hell.display();
  
  delay(1000);
}




