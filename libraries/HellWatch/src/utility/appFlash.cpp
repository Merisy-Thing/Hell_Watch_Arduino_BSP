#include <Arduino.h>
#include <SPI.h>
#include <errno.h>
#include "appFlash.h"

//COMMANDS
#define W_EN 	0x06	//write enable
#define W_DE	0x04	//write disable
#define R_SR1	0x05	//read status reg 1
#define R_SR2	0x35	//read status reg 2
#define W_SR	0x01	//write status reg
#define PAGE_PGM	0x02	//page program
#define QPAGE_PGM	0x32	//quad input page program
#define BLK_E_64K	0xD8	//block erase 64KB
#define BLK_E_32K	0x52	//block erase 32KB
#define SECTOR_E	0x20	//sector erase 4KB
#define CHIP_ERASE	0xc7	//chip erase
#define CHIP_ERASE2	0x60	//=CHIP_ERASE
#define E_SUSPEND	0x75	//erase suspend
#define E_RESUME	0x7a	//erase resume
#define PDWN		0xb9	//power down
#define HIGH_PERF_M	0xa3	//high performance mode
#define CONT_R_RST	0xff	//continuous read mode reset
#define RELEASE		0xab	//release power down or HPM/Dev ID (deprecated)
#define R_MANUF_ID	0x90	//read Manufacturer and Dev ID (deprecated)
#define R_UNIQUE_ID	0x4b	//read unique ID (suggested)
#define R_JEDEC_ID	0x9f	//read JEDEC ID = Manuf+ID (suggested)
#define READ		0x03
#define FAST_READ	0x0b

#define SR1_BUSY_MASK	0x01
#define SR1_WEN_MASK	0x02

#define WINBOND_MANUF	0xef

#define DEFAULT_TIMEOUT 200
  
uint16_t AppFlashClass::readSR()
{
  uint8_t r1,r2;
  select();
  transfer(R_SR1);
  r1 = transfer(0xff);
  deselect();
  deselect();//some delay
  select();
  transfer(R_SR2);
  r2 = transfer(0xff);
  deselect();
  return (((uint16_t)r2)<<8)|r1;
}

uint8_t AppFlashClass::readManufacturer()
{
  uint8_t c;
  select();
  transfer(R_JEDEC_ID);
  c = transfer(0x00);
  transfer(0x00);
  transfer(0x00);
  deselect();
  return c;
}

bool AppFlashClass::busy()
{
  uint8_t r1;
  select();
  transfer(R_SR1);
  r1 = transfer(0xff);
  deselect();
  if(r1 & SR1_BUSY_MASK)
    return true;
  return false;
}

void AppFlashClass::setWriteEnable(bool cmd)
{
  select();
  transfer( cmd ? W_EN : W_DE );
  deselect();
}

bool AppFlashClass::begin()
{
  select();
  transfer(RELEASE);
  deselect();
  delayMicroseconds(5);//>3us
//  Serial.println("Chip Released");

  return true;
}

void AppFlashClass::end()
{
  select();
  transfer(PDWN);
  deselect();
  delayMicroseconds(5);//>3us
}

uint16_t AppFlashClass::read (uint32_t addr,uint8_t *buf,uint16_t n)
{
  if(busy())
    return 0;
  
  select();
  transfer(READ);
  transfer(addr>>16);
  transfer(addr>>8);
  transfer(addr);
  for(uint16_t i=0;i<n;i++)
  {
    buf[i] = transfer(0x00);
  }
  deselect();
  
  return n;
}

void AppFlashClass::writePage(uint32_t addr_start,uint8_t *buf)
{
  select();
  transfer(PAGE_PGM);
  transfer(addr_start>>16);
  transfer(addr_start>>8);
  transfer(0x00);
  uint8_t i=0;
  do {
    transfer(buf[i]);
    i++;
  }while(i!=0);
  deselect();
}

void AppFlashClass::eraseSector(uint32_t addr_start)
{
  select();
  transfer(SECTOR_E);
  transfer(addr_start>>16);
  transfer(addr_start>>8);
  transfer(addr_start);
  deselect();
}

void AppFlashClass::erase32kBlock(uint32_t addr_start)
{
  select();
  transfer(BLK_E_32K);
  transfer(addr_start>>16);
  transfer(addr_start>>8);
  transfer(addr_start);
  deselect();
}

bool AppFlash::begin()
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

void AppFlash::end()
{
  AppFlashClass::end();
  spi.end();
}



