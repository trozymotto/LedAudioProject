#ifndef _ADAFRUIT_GFX_H
#define _ADAFRUIT_GFX_H

#define swap(a, b) { int16_t t = a; a = b; b = t; }

void Adafruit_GFX(int16_t w, int16_t h);

// These MAY be overridden by the subclass to provide device-specific
// optimized code.  Otherwise 'generic' versions are used.
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void ada_fillScreen(uint16_t color);
void invertDisplay(bool i);

// These exist only with Adafruit_GFX (no subclass overrides)

void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
void drawBitmap_2(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
void drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
void setCursor(int16_t x, int16_t y);
void setTextColor(uint16_t c);
void setTextColor_2(uint16_t c, uint16_t bg);
void setTextSize(uint8_t s);
void setTextWrap(bool w);
void setRotation(uint8_t r);

#if ARDUINO >= 100
size_t write(uint8_t);
#else
void   write(uint8_t);
#endif

const  int16_t height(void);
const  int16_t width(void);

const  uint8_t getRotation(void);

int16_t BOARD_WIDTH, BOARD_HEIGHT;   // This is the 'raw' display w/h - never changes
int16_t _width, _height; // Display w/h as modified by current rotation
int16_t cursor_x, cursor_y;
uint16_t textcolor, textbgcolor;
uint8_t textsize, rotation;
bool wrap; // If set, 'wrap' text at right edge of display

#endif // _ADAFRUIT_GFX_H
