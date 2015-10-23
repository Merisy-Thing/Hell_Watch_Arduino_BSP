#include "core.h"

const uint8_t PROGMEM lcdBootProgram[] = {
  0xAE,  // Display Off
  0XD5,  // Set Display Clock Divisor v
  0xF0,  //   0x80 is default
  0xA8,  // Set Multiplex Ratio v
  0x3F,
  0xD3,  // Set Display Offset v
  0x00,
  0x40,  // Set Start Line (0)
  0x8D,  // Charge Pump Setting v
  0x14,  //   Enable
  // running this next pair twice?
  0x20,  // Set Memory Mode v
  0x00,  //   Horizontal Addressing
  0xA1,  // Set Segment Re-map (A0) | (b0001)
  0xC8,  // Set COM Output Scan Direction
  0xDA,  // Set COM Pins v
  0x12,
  0x81,  // Set Contrast v
  0xCF,
  0xD9,  // Set Precharge
  0xF1,
  0xDB,  // Set VCom Detect
  0x40,
  0xA4,  // Entire Display ON
  0xA6,  // Set normal/inverse display
  0xAF,  // Display On

  0x20,     // set display mode
  0x00,     // horizontal addressing mode

  0x21,     // set col address
  0x00,
  COLUMN_ADDRESS_END,

  0x22, // set page address
  0x00,
  PAGE_ADDRESS_END
};


ArduboyCore::ArduboyCore() {}

// meant to be overridden by subclasses
void ArduboyCore::setup()
{
  boot();
}

void ArduboyCore::boot()
{
#ifdef HELL_WATCH
  bootPins();
  bootLCD();
#else
  #if F_CPU == 8000000L
  slowCPU();
  #endif

  SPI.begin();
  bootPins();
  bootLCD();

  #ifdef SAFE_MODE
  if (getInput() == (LEFT_BUTTON | UP_BUTTON))
    safeMode();
  #endif
#endif

  saveMuchPower();
}

#ifdef HELL_WATCH
void ArduboyCore::oled_transfer(uint8_t c)
{
    USARTD0.DATA= c;  
	while(!(USARTD0.STATUS & (1 << 6)));  
	USARTD0.STATUS |= 1 <<6;
}
#endif

#if F_CPU == 8000000L
// if we're compiling for 8Mhz we need to slow the CPU down because the
// hardware clock on the Arduboy is 16MHz
void ArduboyCore::slowCPU()
{
  uint8_t oldSREG = SREG;
  cli();                // suspend interrupts
  CLKPR = _BV(CLKPCE);  // allow reprogramming clock
  CLKPR = 1;            // set clock divisor to 2 (0b0001)
  SREG = oldSREG;       // restore interrupts
}
#endif

void ArduboyCore::bootPins()
{

#ifdef HELL_WATCH
  USARTD0.BAUDCTRLA = 0x00;//speed up spi rate to 8M bps
  pinMode(PIN_UP_BUTTON, INPUT_PULLUP);
  pinMode(PIN_RIGHT_BUTTON, INPUT_PULLUP);
  pinMode(PIN_DOWN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_LEFT_BUTTON, INPUT_PULLUP);
  pinMode(PIN_A_BUTTON, INPUT_PULLUP);
  pinMode(PIN_B_BUTTON, INPUT_PULLUP);
#else
  // OLED SPI
  pinMode(DC, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(RST, OUTPUT);
  digitalWrite(RST, HIGH);
  delay(1);           // VDD (3.3V) goes high at start, lets just chill for a ms
  digitalWrite(RST, LOW);   // bring reset low
  delay(10);          // wait 10ms
  digitalWrite(RST, HIGH);  // bring out of reset

  // Buttons
  pinMode(PIN_LEFT_BUTTON, INPUT_PULLUP);
  pinMode(PIN_RIGHT_BUTTON, INPUT_PULLUP);
  pinMode(PIN_UP_BUTTON, INPUT_PULLUP);
  pinMode(PIN_DOWN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_A_BUTTON, INPUT_PULLUP);
  pinMode(PIN_B_BUTTON, INPUT_PULLUP);
#endif
}

void ArduboyCore::bootLCD()
{
#ifndef HELL_WATCH
  // setup the ports we need to talk to the OLED
  csport = portOutputRegister(digitalPinToPort(CS));
  cspinmask = digitalPinToBitMask(CS);
  dcport = portOutputRegister(digitalPinToPort(DC));
  dcpinmask = digitalPinToBitMask(DC);

  SPI.setClockDivider(SPI_CLOCK_DIV2);

  LCDCommandMode();
  for (int8_t i=0; i < sizeof(lcdBootProgram); i++) {
    SPI.transfer(pgm_read_byte(lcdBootProgram + i));
  }
#endif
  LCDDataMode();
}

void ArduboyCore::LCDDataMode()
{
#ifdef HELL_WATCH
    digitalWrite(DC, LOW);
#else
  *dcport |= dcpinmask;
  *csport &= ~cspinmask;
#endif
}

void ArduboyCore::LCDCommandMode()
{
#ifdef HELL_WATCH
    digitalWrite(DC, HIGH);
#else
  *csport |= cspinmask; // why are we doing this twice?
  *csport |= cspinmask;
  *dcport &= ~dcpinmask;
  *csport &= ~cspinmask;
#endif
}



void ArduboyCore::safeMode()
{
  blank(); // too avoid random gibberish
  while (true) {
    asm volatile("nop \n");
  }
}


/* Power Management */

void ArduboyCore::idle()
{
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_mode();
}

void ArduboyCore::saveMuchPower()
{
#ifndef HELL_WATCH
  power_adc_disable();
  power_usart0_disable();
  power_twi_disable();
  // timer 0 is for millis()
  // timers 1 and 3 are for music and sounds
  power_timer2_disable();
  power_usart1_disable();
  // we need USB, for now (to allow triggered reboots to reprogram)
  // power_usb_disable()
#endif
}

uint8_t ArduboyCore::width() { return WIDTH; }

uint8_t ArduboyCore::height() { return HEIGHT; }


/* Drawing */

void ArduboyCore::paint8Pixels(uint8_t pixels)
{
#ifdef HELL_WATCH
  oled_transfer(pixels);
#else
  SPI.transfer(pixels);
#endif
}

void ArduboyCore::paintScreen(const unsigned char *image)
{
#ifdef HELL_WATCH
  digitalWrite(CS, HIGH);
  digitalWrite(DC, HIGH);
  digitalWrite(CS, LOW);
  for (int a = 0; a < (HEIGHT*WIDTH)/8; a++){
    oled_transfer(pgm_read_byte(image + a));
  }
  digitalWrite(CS, HIGH);
#else
  for (int i = 0; i < (HEIGHT*WIDTH)/8; i++)
  {
    SPI.transfer(pgm_read_byte(image + i));
  }
#endif
}

// paint from a memory buffer, this should be FAST as it's likely what
// will be used by any buffer based subclass
void ArduboyCore::paintScreen(unsigned char image[])
{
#ifdef HELL_WATCH
  digitalWrite(CS, HIGH);
  digitalWrite(DC, HIGH);
  digitalWrite(CS, LOW);
  for (int a = 0; a < (HEIGHT*WIDTH)/8; a++){
    oled_transfer(image[a]);
  }
  digitalWrite(CS, HIGH);
#else
  for (int i = 0; i < (HEIGHT*WIDTH)/8; i++)
  {
    // SPI.transfer(image[i]);

    // we need to burn 18 cycles between sets of SPDR
    // 4 clock cycles
    SPDR = image[i];
    // 7 clock cycles
    asm volatile(
      "mul __zero_reg__, __zero_reg__ \n" // 2 cycles
      "mul __zero_reg__, __zero_reg__ \n" // 2 cycles
      "mul __zero_reg__, __zero_reg__ \n" // 2 cycles
      );
  }
#endif
}

void ArduboyCore::blank()
{
#ifdef HELL_WATCH
  digitalWrite(CS, LOW);
  for (int a = 0; a < (HEIGHT*WIDTH)/8; a++) oled_transfer(0x00);
  digitalWrite(CS, HIGH);
#else
  for (int i = 0; i < (HEIGHT*WIDTH)/8; i++)
    SPI.transfer(0x00);
#endif
}


/* Buttons */

uint8_t ArduboyCore::getInput()
{
#ifdef HELL_WATCH
  return 0;
#else
  // using ports here is ~100 bytes smaller than digitalRead()
  #ifdef DEVKIT
  // down, left, up
  uint8_t buttons = ((~PINB) & B01110000);
  // right button
  buttons = buttons | (((~PINC) & B01000000) >> 4);
  // A and B
  buttons = buttons | (((~PINF) & B11000000) >> 6);
  #endif

  // b0dlu0rab - see button defines in Arduboy.h
  return buttons;
#endif
}
