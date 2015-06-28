#ifndef _HWFLASH_H__
#define _HWFLASH_H__

#include <inttypes.h>
#include <SPI.h>
#include <./utility/appFlash.h>

class Flash: public AppFlashClass {
private:
  SPIClass spi;

  inline void select() {
    digitalWrite(SPI_ROM_SS,LOW);
  }

  inline uint8_t transfer(uint8_t x) {
    byte y = spi.transfer(x);
    return y;
  }

  inline void deselect() {
    digitalWrite(SPI_ROM_SS,HIGH);
  }

public:
  bool begin();
  void end();
};

#endif

