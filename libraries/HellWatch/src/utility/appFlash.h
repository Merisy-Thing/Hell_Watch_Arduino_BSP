#ifndef _APPFLASH_H__
#define _APPFLASH_H__

#include <inttypes.h>
#include <SPI.h>

class AppFlashClass {
public:  
  bool begin();
  void end();


  uint16_t read(uint32_t addr,uint8_t *buf,uint16_t n=1);

  void setWriteEnable(bool cmd = true);
  inline void WE(bool cmd = true) {setWriteEnable(cmd);}
  
  //WE() every time before write or erase
  
  void writePage(uint32_t addr_start,uint8_t *buf);//addr is 8bit-aligned, 0x00ffff00
  //write a page, sizeof(buf) is 256 bytes

  void eraseSector(uint32_t addr);//addr is 12bit-aligned, 0x00fff000
  //erase a sector ( 4096bytes ), return false if error

  void erase32kBlock(uint32_t addr);//addr is 15bit-aligned, 0x00ff8000
  //erase a 32k block ( 32768b )

  bool busy();
  
  uint8_t  readManufacturer();
  uint16_t readSR();

protected:
  virtual void select() = 0;
  virtual uint8_t transfer(uint8_t x) = 0;
  virtual void deselect() = 0;
  
};


#endif

