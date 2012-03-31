/*********************************************************************
This is a library for our Monochrome OLEDs based on SSD1305 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

These displays use SPI to communicate, 4 or 5 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/


#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Adafruit_GFX.h>

#define swap(a, b) { uint8_t t = a; a = b; b = t; }

#define BLACK 0
#define WHITE 1

/*=========================================================================
    SSD1305 Displays
    -----------------------------------------------------------------------
    The driver is used in multiple displays (128x64, 128x32, etc.).
    Select the appropriate display below to create an appropriately
    sized framebuffer, etc.

    SSD1305_128_64  128x64 pixel display

    SSD1305_128_32  128x32 pixel display

    You also need to set the LCDWIDTH and LCDHEIGHT defines to an 
    appropriate size

    -----------------------------------------------------------------------*/
#define SSD1305_128_32
/*=========================================================================*/

#if defined SSD1305_128_64 && defined SSD1305_128_32
  #error "Only one SSD1305 display can be specified at once in SSD1305.h"
#endif
#if !defined SSD1305_128_64 && !defined SSD1305_128_32
  #error "At least one SSD1305 display must be specified in SSD1305.h"
#endif

#if defined SSD1305_128_64
  #define SSD1305_LCDWIDTH                  128
  #define SSD1305_LCDHEIGHT                 64
#endif
#if defined SSD1305_128_32
  #define SSD1305_LCDWIDTH                  128
  #define SSD1305_LCDHEIGHT                 32
#endif

#define SSD1305_SETLOWCOLUMN 0x00
#define SSD1305_SETHIGHCOLUMN 0x10
#define SSD1305_MEMORYMODE 0x20
#define SSD1305_SETCOLADDR 0x21
#define SSD1305_SETPAGEADDR 0x22
#define SSD1305_SETSTARTLINE 0x40

#define SSD1305_SETCONTRAST 0x81
#define SSD1305_SETBRIGHTNESS 0x82

#define SSD1305_SETLUT 0x91

#define SSD1305_SEGREMAP 0xA0
#define SSD1305_DISPLAYALLON_RESUME 0xA4
#define SSD1305_DISPLAYALLON 0xA5
#define SSD1305_NORMALDISPLAY 0xA6
#define SSD1305_INVERTDISPLAY 0xA7
#define SSD1305_SETMULTIPLEX 0xA8
#define SSD1305_DISPLAYDIM 0xAC
#define SSD1305_MASTERCONFIG 0xAD
#define SSD1305_DISPLAYOFF 0xAE
#define SSD1305_DISPLAYON 0xAF

#define SSD1305_SETPAGESTART 0xB0

#define SSD1305_COMSCANINC 0xC0
#define SSD1305_COMSCANDEC 0xC8
#define SSD1305_SETDISPLAYOFFSET 0xD3
#define SSD1305_SETDISPLAYCLOCKDIV 0xD5
#define SSD1305_SETAREACOLOR 0xD8
#define SSD1305_SETPRECHARGE 0xD9
#define SSD1305_SETCOMPINS 0xDA
#define SSD1305_SETVCOMLEVEL 0xDB


class Adafruit_SSD1305 : public Adafruit_GFX {
 public:
  Adafruit_SSD1305(int8_t SID, int8_t SCLK, int8_t DC, int8_t RST, int8_t CS) :sid(SID), sclk(SCLK), dc(DC), rst(RST), cs(CS) {}
    Adafruit_SSD1305(int8_t SID, int8_t SCLK, int8_t DC, int8_t RST) :sid(SID), sclk(SCLK), dc(DC), rst(RST), cs(-1) {}


  void begin(void);
  void command(uint8_t c);
  void data(uint8_t c);

  void clearDisplay(void);
  void invertDisplay(uint8_t i);
  void setBrightness(uint8_t i);
  void display();

  void drawPixel(uint16_t x, uint16_t y, uint16_t color);

 private:
  int8_t sid, sclk, dc, rst, cs;
  void fastSPIwrite(uint8_t c);
  void slowSPIwrite(uint8_t c);

  volatile uint8_t *mosiport, *clkport, *csport, *dcport;
  uint8_t mosipinmask, clkpinmask, cspinmask, dcpinmask;

};
