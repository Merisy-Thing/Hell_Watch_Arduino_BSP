/*********************************************************************
This is a library for Hell Watch SPI Flash

  ------> http://www.hellprototypes.com/

BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution

*********************************************************************/
#include <Arduino.h>
#include <SPI.h>
#include <errno.h>
#include "hwFlash.h"

bool Flash::begin()
{
  SPIPortSettings spiPort(&SPI_PORT, SPI_ROM_SS, SPI_MOSI, SPI_MISO, SPI_SCK);

  spi.setConfiguration(spiPort);

  spi.begin();
  spi.setBitOrder(MSBFIRST);
  spi.setClockDivider(SPI_CLOCK_DIV2);
  spi.setDataMode(SPI_MODE0);
  deselect();
  //Serial.println("SPI OK");

  return AppFlashClass::begin();
}

void Flash::end()
{
  AppFlashClass::end();
  spi.end();
}



