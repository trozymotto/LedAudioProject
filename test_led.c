#include <inttypes.h> //gives us uintX_t
#include <stdbool.h>
#include <math.h>
#include <pololu/orangutan.h>
#include "Adafruit_GFX.h"
#include "RGBmatrixPanel.h"

void test_shapes()
{
  // draw a pixel in solid white
  drawPixel(0, 0, Color333(7, 7, 7));
  delay_ms(500);

  // fix the screen with green
  fillRect(0, 0, 32, 32, Color333(0, 7, 0));
  delay_ms(500);

  // draw a box in yellow
  drawRect(0, 0, 32, 32, Color333(7, 7, 0));
  delay_ms(500);

  // draw an 'X' in red
  drawLine(0, 0, 31, 31, Color333(7, 0, 0));
  drawLine(31, 0, 0, 31, Color333(7, 0, 0));
  delay_ms(500);

  // draw a blue circle
  drawCircle(10, 10, 10, Color333(0, 0, 7));
  delay_ms(500);

  // fill a violet circle
  fillCircle(21, 21, 10, Color333(7, 0, 7));
  delay_ms(500);

  // fill the screen with 'black'
  fillScreen(Color333(0, 0, 0));

  // draw some text!
  setCursor(1, 0);    // start at top left, with one pixel of spacing
  setTextSize(1);     // size 1 == 8 pixels high
  setTextWrap(false); // Don't wrap at end of line - will do ourselves

  // print each letter with a rainbow color
  setTextColor(Color333(7,0,0));
  write('B');
  setTextColor(Color333(7,4,0));
  write('A');
  setTextColor(Color333(7,7,0));
  write('R');
  setTextColor(Color333(4,7,0));
  write('R');
  setTextColor(Color333(0,7,0));
  write('Y');

  setCursor(1, 9);

  setTextColor(Color333(0,7,7));
  write('R');
  setTextColor(Color333(0,4,7));
  write('O');
  setTextColor(Color333(0,0,7));
  write('C');
  setTextColor(Color333(4,0,7));
  write('K');
  setTextColor(Color333(7,0,4));
  write('S');
  delay_ms(500);

  // fill the screen with 'black'
  fillScreen(Color333(0, 0, 0));

}

void test_color_wheel()
{
  int      x, y, hue;
  float    dx, dy, d;
	uint8_t  sat, val;
	uint16_t c;

  for(y=0; y < width(); y++) {
  dy = 15.5 - (float)y;
    for(x=0; x < height(); x++) {
      dx = 15.5 - (float)x;
      d  = dx * dx + dy * dy;
      if(d <= (16.5 * 16.5)) { // Inside the circle(ish)?
        hue = (int)((atan2(-dy, dx) + M_PI) * 1536.0 / (M_PI * 2.0));
        d = sqrt(d);
        if(d > 15.5) {
          // Do a little pseudo anti-aliasing along perimeter
          sat = 255;
          val = (int)((1.0 - (d - 15.5)) * 255.0 + 0.5);
        } else
        {
          // White at center
          sat = (int)(d / 15.5 * 255.0 + 0.5);
          val = 255;
        }
        c = ColorHSV(hue, sat, val, true);
      } else {
        c = 0;
      }
      drawPixel(x, y, c);
    }
  }
}
