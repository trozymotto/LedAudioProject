#include "Adafruit_GFX.h"

 void begin(void);
 void drawPixel(int16_t x, int16_t y, uint16_t c);
 void fillScreen(uint16_t c);
 void updateDisplay(void);
 void swapBuffers(bool);
 void dumpMatrix(void);

 uint8_t *backBuffer(void);

 uint16_t Color333(uint8_t r, uint8_t g, uint8_t b);
 uint16_t Color444(uint8_t r, uint8_t g, uint8_t b);
 uint16_t Color888(uint8_t r, uint8_t g, uint8_t b);
 uint16_t Color888_2(uint8_t r, uint8_t g, uint8_t b, bool gflag);
 uint16_t ColorHSV(long hue, uint8_t sat, uint8_t val, bool gflag);

 uint8_t         *matrixbuff[2];
 uint8_t          nRows;
 volatile uint8_t backindex;
 volatile bool swapflag;

 // Init/alloc code common to both constructors:
 void init(uint8_t rows, uint8_t a, uint8_t b, uint8_t c, uint8_t d,
     uint8_t sclk, uint8_t latch, uint8_t oe, bool dbuf,
     uint8_t width);

 // PORT register pointers, pin bitmasks, pin numbers:
 volatile uint8_t *latport;
 volatile uint8_t *oeport;
 volatile uint8_t *addraport;
 volatile uint8_t *addrbport;
 volatile uint8_t *addrcport;
 volatile uint8_t *addrdport;

 uint8_t sclkpin;
 uint8_t latpin;
 uint8_t oepin;
 uint8_t addrapin;
 uint8_t addrbpin;
 uint8_t addrcpin;
 uint8_t addrdpin;
 uint8_t _sclk;
 uint8_t _latch;
 uint8_t _oe;
 uint8_t _a;
 uint8_t _b;
 uint8_t _c;
 uint8_t _d;

 // Counters/pointers for interrupt handler:
 volatile uint8_t row;
 volatile uint8_t plane;
 volatile uint8_t *buffptr;
