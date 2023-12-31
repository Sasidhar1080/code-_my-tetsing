#include <Adafruit_SH1106.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

#define OLED_SDA 21
#define OLED_SCL 22

Adafruit_SH1106 display(21, 22);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SH1106.h!");
#endif


// Bitmap of  Image
const unsigned char Ganesh [] PROGMEM = {
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x18, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x07, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x04, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x20, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x40, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x09, 0x80, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x01, 0xfc, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x07, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x7f, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x03, 0xe7, 0xe1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x1f, 0x1f, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0xfe, 0x07, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x3f, 0x27, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x1c, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x01, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x03, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x0f, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x1f, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x1f, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x0f, 0x81, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x04, 0x03, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x01, 0x80, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x1e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x47, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x7f, 0xff, 0x7f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xd0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x84, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x8c, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xe0, 0x00, 0x00, 0x63, 0x00, 0x00, 0x1f, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0x80, 0x00, 0x00, 0x23, 0x02, 0x45, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xfe, 0x00, 0x00, 0x00, 0x61, 0x83, 0x22, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf0, 0x00, 0x00, 0x00, 0x71, 0xf1, 0x91, 0xef, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 
  0xff, 0xc0, 0x00, 0x00, 0x00, 0x79, 0xf9, 0x99, 0xbf, 0xdf, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 
  0xff, 0xc0, 0x00, 0x00, 0x00, 0x79, 0xf9, 0xcc, 0xff, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 
  0xff, 0xf0, 0x00, 0x00, 0x00, 0x78, 0xf9, 0xe6, 0x6f, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 
  0xff, 0xe0, 0x00, 0x00, 0x00, 0x7c, 0xf9, 0xe7, 0x3f, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xff
};
const unsigned char happybirthday [] PROGMEM = {
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xe1, 0xff, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xfe, 0x7f, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xfc, 0xe1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0x9e, 0x19, 0xe4, 0x71, 0x8e, 0x63, 0xde, 0x7f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0xf3, 0x9e, 0xf3, 0xcc, 0xf3, 0x9c, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf9, 0x3f, 0xf3, 0x3c, 0xe7, 0xcc, 0xf3, 0x3c, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x9f, 0x7f, 0xe6, 0x7d, 0xe7, 0x99, 0xf2, 0x79, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf9, 0xfe, 0x7f, 0xe6, 0x79, 0xcf, 0x99, 0xe6, 0x79, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xcf, 0xfc, 0xff, 0xcc, 0xf3, 0x8f, 0x31, 0xcc, 0xf3, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0x3f, 0xf9, 0xff, 0x9e, 0x10, 0x80, 0xf0, 0x1c, 0x03, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xfc, 0xff, 0xfb, 0xff, 0x9f, 0xff, 0xbf, 0xe7, 0xff, 0xe7, 0xfe, 0x3f, 0xff, 0xff, 
  0xff, 0xff, 0xf3, 0xff, 0xf3, 0xff, 0x3f, 0xff, 0x20, 0x01, 0xff, 0xe7, 0x3f, 0xe3, 0xff, 0xff, 
  0xff, 0xff, 0xe7, 0xff, 0xe7, 0xff, 0x7f, 0xe2, 0x7f, 0xcf, 0x87, 0xc3, 0xff, 0xf8, 0xff, 0xff, 
  0xff, 0xff, 0xcf, 0xff, 0xcf, 0xfe, 0x7f, 0x3e, 0x7f, 0x9f, 0xf8, 0xcf, 0xff, 0xfe, 0x7f, 0xff, 
  0xff, 0xff, 0xcf, 0xf8, 0x01, 0xfc, 0xfc, 0xfe, 0xff, 0x9f, 0xfe, 0x1d, 0xff, 0xff, 0x3f, 0xff, 
  0xff, 0xff, 0xc7, 0xc7, 0x7c, 0x7c, 0xfd, 0xfc, 0xff, 0x3f, 0xfc, 0x18, 0xff, 0xff, 0x9e, 0xff, 
  0xff, 0xff, 0xf1, 0x3f, 0xfe, 0x39, 0xfd, 0xfd, 0xff, 0x3f, 0xe7, 0x32, 0xff, 0xff, 0xdf, 0x3f, 
  0xff, 0xff, 0xfc, 0x0f, 0xfe, 0x1b, 0xfc, 0x79, 0xfe, 0x78, 0xff, 0x37, 0xff, 0xff, 0xff, 0x1f, 
  0xff, 0xff, 0xfd, 0xf9, 0xfe, 0x33, 0xfe, 0x00, 0x00, 0x0f, 0xff, 0xe7, 0xff, 0xff, 0xff, 0x9f, 
  0xff, 0xff, 0xfd, 0xf3, 0xfc, 0x70, 0xff, 0xc0, 0x04, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0x8f, 
  0xff, 0xf3, 0xff, 0xe7, 0xf1, 0xf9, 0xff, 0xe3, 0xfc, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0x8f, 
  0xff, 0x8f, 0xff, 0xe7, 0x8f, 0xfb, 0x9f, 0xe7, 0xf9, 0xc7, 0xfb, 0x9f, 0xbf, 0xfb, 0xff, 0x8f, 
  0xfe, 0x3f, 0xff, 0xcc, 0xc7, 0xe7, 0x83, 0xc7, 0xf1, 0x23, 0xcf, 0x39, 0x8c, 0xf3, 0xff, 0x8f, 
  0xfc, 0xff, 0xff, 0x9f, 0xf3, 0xe7, 0x67, 0x8f, 0xe1, 0xe7, 0x9f, 0x37, 0x9c, 0xe7, 0xff, 0x8f, 
  0xf9, 0xff, 0xff, 0x9f, 0xf1, 0xce, 0xe7, 0x8f, 0xc3, 0xe7, 0x3e, 0x4f, 0x39, 0xc7, 0xff, 0x8f, 
  0xfb, 0xff, 0xff, 0x3f, 0xf1, 0x9d, 0xcf, 0x9f, 0xa7, 0xcf, 0x7e, 0xdf, 0x33, 0xcf, 0xff, 0x0f, 
  0xf3, 0xff, 0xfe, 0x3f, 0xf1, 0x9b, 0x9d, 0x9f, 0x67, 0xce, 0x7d, 0x9e, 0x73, 0x9f, 0xff, 0x1f, 
  0xf3, 0xff, 0xfe, 0x7f, 0xf3, 0x37, 0x9b, 0x1c, 0xcf, 0x9e, 0x7b, 0x9e, 0xe7, 0x1f, 0xfe, 0x3f, 
  0xf3, 0xff, 0xfc, 0xff, 0xe7, 0x1f, 0x8f, 0x03, 0x8f, 0x8f, 0x07, 0x84, 0x62, 0x3f, 0xfc, 0x7f, 
  0xf1, 0xff, 0xfd, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xf8, 0xff, 
  0xf1, 0xff, 0xff, 0xff, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x01, 0xff, 0xfc, 0x7f, 0xe3, 0xff, 
  0xf0, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0x07, 0xfc, 0xff, 0x8f, 0xff, 
  0xf8, 0x7f, 0xff, 0xfc, 0x7f, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xff, 0xf8, 0xf9, 0xfc, 0x7f, 0xff, 
  0xfc, 0x3f, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0x39, 0xe3, 0xff, 0xff, 
  0xfe, 0x0f, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 
  0xff, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xff, 0xfc, 0x03, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x3f, 0xe7, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

void setup()   
{                
  pinMode(OLED_SDA, INPUT_PULLUP);           // set pin to input
  pinMode(OLED_SCL, INPUT_PULLUP);           // set pin to input
  Serial.begin(115200);

 display.begin(SH1106_SWITCHCAPVCC, 0x3C); 
  display.display();
  delay(2000);
  // Invert Display
  //display.invertDisplay(1);
}

void loop() {
    // Clear the buffer.

      display.clearDisplay();

  // Display bitmap
  display.drawBitmap(0, 0,happybirthday, 128, 64, WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();

  // Display bitmap
  display.drawBitmap(0, 0,Ganesh, 128, 64, WHITE);
  display.display();
  delay(3000);

///happy birthday
     display.clearDisplay();


    display.setCursor(0,0);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println("HAPPY");

    display.setCursor(0,22);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println("BIRTHDAY!");

    display.setCursor(0,44);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println("GANESH BRO");
    display.display();
    delay(2000);


    
  }
