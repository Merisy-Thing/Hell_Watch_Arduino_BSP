#ifndef _HW_DISPLAY_h
#define _HW_DISPLAY_h

#include <Arduino.h>
#include <Print.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <limits.h>


#define PIXEL_SAFE_MODE

#define CS 16
#define DC 18
#define RST 20

#define WIDTH 128
#define HEIGHT 64

#define WHITE 1
#define BLACK 0

class Display : public Print
{
public:
  Display();

  void blank();
  void clearDisplay();
  void display();
  void drawScreen(const unsigned char *image);
  void drawScreen(unsigned char image[]);
  void drawPixel(int x, int y, uint8_t color);
  uint8_t getPixel(uint8_t x, uint8_t y);
  void drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
  void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint8_t color);
  void fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
  void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint8_t color);
  void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);
  void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint8_t color);
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint8_t color);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
  void fillScreen(uint8_t color);
  void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color);
  void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color);
  void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);
  void fillTriangle (int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);
  void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t color);
  void drawSlowXYBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t color);
  void drawChar(int16_t x, int16_t y, unsigned char c, uint8_t color, uint8_t bg, uint8_t size);
  void setCursor(int16_t x, int16_t y);
  void setTextSize(uint8_t s);
  void setTextWrap(boolean w);
  inline unsigned char* getBuffer();
  uint8_t width();
  uint8_t height();
  virtual size_t write(uint8_t);
  void swap(int16_t& a, int16_t& b);

private:
  unsigned char sBuffer[(HEIGHT*WIDTH)/8];
  void oled_transfer(uint8_t c);

// Adafruit stuff
protected:
  int16_t cursor_x = 0;
  int16_t cursor_y = 0;
  uint8_t textsize = 1;
  boolean wrap; // If set, 'wrap' text at right edge of display
};

#endif
