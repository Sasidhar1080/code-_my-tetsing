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

const unsigned char iiith [] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0xe0, 0x0c, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0xff, 0xe7, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xe3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x81, 0x38, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xfc, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0xc1, 0xff, 0xff, 0xff, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0x2f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xef, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfe, 0xff, 0xff, 0x1f, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0x8e, 0x7d, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0xcf, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0x8f, 0xff, 0xff, 0xff, 0xf1, 0xfe, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0x8f, 0xff, 0xfd, 0xbf, 0xff, 0xfd, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x17, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xfd, 0xde, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xfe, 0xfc, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0xcf, 0xff, 0xef, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x10, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xcf, 0xf8, 0xff, 0xfb, 0xfb, 0xff, 0xf0, 0x04, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe7, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0x2c, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xcf, 0xfc, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x06, 0xff, 0xff, 0x0f, 0xfe, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x02, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xf6, 0xff, 0xdf, 0xff, 0xc0, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x77, 0xff, 0xff, 0xff, 0xf8, 0x7e, 0xf8, 0x3f, 0xff, 0xff, 0x7f, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x47, 0xff, 0xff, 0xff, 0xf8, 0xfe, 0xfc, 0x3f, 0xff, 0xff, 0xdf, 0x00, 0x00, 
0x00, 0x00, 0x00, 0xdb, 0xff, 0xdf, 0xff, 0xf1, 0x3e, 0x3f, 0xff, 0xff, 0xff, 0x9b, 0x00, 0x00, 
0x00, 0x00, 0x01, 0xff, 0xff, 0xbf, 0xff, 0xf0, 0x29, 0x3f, 0xff, 0xff, 0xff, 0xcd, 0x00, 0x00, 
0x00, 0x00, 0x05, 0xff, 0xff, 0xf3, 0xff, 0x84, 0x70, 0x07, 0xff, 0xff, 0xff, 0xc7, 0x80, 0x00, 
0x00, 0x00, 0x03, 0xff, 0xff, 0x83, 0xff, 0xc6, 0xe6, 0xc1, 0xff, 0xff, 0xff, 0xc7, 0x80, 0x00, 
0x00, 0x00, 0x07, 0xff, 0xfe, 0xc3, 0xff, 0xe6, 0xe7, 0xc1, 0xff, 0xf7, 0xff, 0xef, 0xe0, 0x00, 
0x00, 0x00, 0xff, 0xff, 0xfa, 0xc7, 0xff, 0xdf, 0x97, 0xc1, 0xff, 0xe7, 0xff, 0xf7, 0xa0, 0x00, 
0x00, 0x01, 0xcf, 0xff, 0x83, 0xc4, 0xff, 0xff, 0xe7, 0xcc, 0x7f, 0xfb, 0xff, 0xfe, 0x00, 0x00, 
0x00, 0x01, 0x8f, 0xff, 0x9f, 0xc5, 0xff, 0xff, 0xe5, 0xfe, 0x3f, 0xfb, 0xff, 0xfe, 0x00, 0x00, 
0x00, 0x00, 0xff, 0xff, 0xff, 0xe3, 0xff, 0xff, 0xef, 0xff, 0x3f, 0xef, 0xff, 0xff, 0x80, 0x00, 
0x00, 0x01, 0xff, 0xff, 0xff, 0xe3, 0xff, 0xff, 0xef, 0xe7, 0x3f, 0xff, 0xff, 0xff, 0x00, 0x00, 
0x00, 0x01, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xe5, 0x7f, 0xfe, 0x7f, 0xf8, 0x00, 0x00, 
0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xf3, 0x7f, 0xfc, 0x00, 0x00, 
0x00, 0x01, 0xff, 0xff, 0x7f, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xf8, 0x00, 0x00, 
0x00, 0x00, 0x7f, 0xe6, 0x7f, 0xd7, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xe0, 0xa1, 0xec, 0x00, 0x00, 
0x00, 0x00, 0x07, 0x87, 0xff, 0x3f, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xc3, 0xc5, 0xed, 0x00, 0x00, 
0x00, 0x00, 0x02, 0x06, 0x78, 0x3f, 0xff, 0xbf, 0xff, 0xff, 0xff, 0xef, 0xcf, 0xf9, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x06, 0x7f, 0xfc, 0xff, 0xff, 0xff, 0xef, 0xff, 0xef, 0xff, 0x90, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x06, 0x17, 0xfc, 0x37, 0xff, 0x3f, 0x7b, 0xff, 0xef, 0xbf, 0xc0, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x06, 0x13, 0xfc, 0x3f, 0xff, 0x3f, 0x4f, 0xff, 0xef, 0x00, 0x40, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x06, 0x12, 0x4c, 0x7e, 0x3f, 0xff, 0x59, 0xfc, 0xf0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x04, 0x12, 0x48, 0x3e, 0x3f, 0xfe, 0x19, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x02, 0x10, 0x48, 0x2e, 0x3f, 0xb0, 0x00, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x06, 0x00, 0x08, 0x0e, 0x1f, 0xf4, 0x01, 0x80, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x04, 0x00, 0x08, 0x08, 0x1f, 0x86, 0x00, 0x80, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x04, 0x00, 0x08, 0x00, 0x1f, 0x82, 0x00, 0x80, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x02, 0x3f, 0xc2, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x3f, 0xe6, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xfe, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x38, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    
const unsigned char sample_co2 [] PROGMEM = {
  0xff, 0xff, 0xfb, 0xff, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xe3, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xf1, 0xff, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xf3, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xc7, 
0xff, 0x3f, 0xff, 0xfb, 0xff, 0xff, 0xcf, 0xc7, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xdf, 0xff, 0xff, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x1f, 0xff, 0xf3, 0xff, 
0xff, 0xf1, 0xfe, 0x00, 0x0f, 0xff, 0xf3, 0xff, 0xff, 0xf1, 0xfc, 0x00, 0x07, 0xff, 0xff, 0xf7, 
0xff, 0xf1, 0xf8, 0x00, 0x03, 0xff, 0xff, 0xf7, 0xf7, 0xfb, 0xf0, 0x00, 0x01, 0xff, 0xff, 0xff, 
0xf7, 0xff, 0xf0, 0x00, 0x00, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x1f, 0xff, 
0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x07, 0xff, 
0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 
0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 
0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 
0xf8, 0x00, 0x1f, 0xe0, 0x0f, 0xf0, 0x00, 0x7f, 0xf8, 0x00, 0x3f, 0xf8, 0x3f, 0xf8, 0x00, 0x7f, 
0xf0, 0x00, 0x7f, 0xf8, 0x7f, 0xfc, 0x00, 0x3f, 0xf0, 0x00, 0xff, 0xf8, 0xff, 0xfe, 0x00, 0x3f, 
0xe0, 0x01, 0xfc, 0xf8, 0xfc, 0x7e, 0x00, 0x3f, 0xe0, 0x01, 0xf8, 0x11, 0xf8, 0x3f, 0x00, 0x1f, 
0xc0, 0x03, 0xf0, 0x01, 0xf0, 0x1f, 0x00, 0x0f, 0xc0, 0x03, 0xf0, 0x01, 0xf0, 0x1f, 0x00, 0x07, 
0xc0, 0x03, 0xe0, 0x01, 0xf0, 0x1f, 0x80, 0x07, 0xc0, 0x03, 0xe0, 0x03, 0xf0, 0x1f, 0x80, 0x03, 
0x80, 0x03, 0xe0, 0x03, 0xf0, 0x0f, 0x80, 0x03, 0x80, 0x03, 0xe0, 0x01, 0xf0, 0x1f, 0x80, 0x01, 
0x80, 0x03, 0xe0, 0x01, 0xf0, 0x1f, 0x00, 0x01, 0x80, 0x03, 0xf0, 0x01, 0xf0, 0x1f, 0x3e, 0x00, 
0x80, 0x01, 0xf0, 0x01, 0xf0, 0x1f, 0x3f, 0x00, 0x80, 0x01, 0xf8, 0x31, 0xf8, 0x3f, 0x37, 0x00, 
0xc0, 0x00, 0xff, 0xf8, 0xff, 0xfe, 0x07, 0x00, 0xc0, 0x00, 0xff, 0xf8, 0x7f, 0xfc, 0x07, 0x00, 
0xc0, 0x00, 0x7f, 0xf8, 0x3f, 0xfc, 0x0e, 0x00, 0xc0, 0x00, 0x3f, 0xf0, 0x1f, 0xf0, 0x1c, 0x00, 
0xe0, 0x00, 0x07, 0xc0, 0x07, 0xc0, 0x3f, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x01, 
0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 
0xff, 0x80, 0x00, 0x00, 0x00, 0x10, 0x00, 0x1f, 0xff, 0xc0, 0x00, 0xf0, 0x00, 0x3c, 0x00, 0x3f, 
0xff, 0xf0, 0x03, 0xfc, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

const unsigned char temp [] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x87, 0xff, 0xbf, 0xf3, 0xc0, 0xf1, 0xfc, 0x01, 0x8f, 0xff, 0xbf, 0xf3, 0xc0, 0xf1, 0xff, 0x01, 
0x80, 0x70, 0x38, 0x03, 0xe1, 0xf1, 0xcf, 0x01, 0x80, 0x70, 0x38, 0x03, 0xe1, 0xf1, 0x87, 0x01, 
0x80, 0x70, 0x38, 0x03, 0xe3, 0xf1, 0x83, 0x81, 0x80, 0x70, 0x38, 0x03, 0xf3, 0xf1, 0x87, 0x01, 
0x80, 0x70, 0x3f, 0xe3, 0xb3, 0x71, 0x87, 0x01, 0x80, 0x70, 0x3f, 0xe3, 0xb7, 0x71, 0xff, 0x01, 
0x80, 0x70, 0x38, 0x03, 0xbf, 0x71, 0xfe, 0x01, 0x80, 0x70, 0x38, 0x03, 0x9e, 0x71, 0xc0, 0x01, 
0x80, 0x70, 0x38, 0x03, 0x9e, 0x71, 0x80, 0x01, 0x80, 0x70, 0x38, 0x03, 0x9e, 0x71, 0x80, 0x01, 
0x80, 0x70, 0x38, 0x03, 0x80, 0x71, 0x80, 0x01, 0x80, 0x70, 0x3f, 0xf3, 0x80, 0x71, 0x80, 0x01, 
0x80, 0x30, 0x3f, 0xf3, 0x80, 0x71, 0x80, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x01, 0x80, 0x01, 0xe7, 0x80, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x01, 0xe3, 0x80, 0x00, 0x00, 0x00, 0x01, 0x80, 0x01, 0xe3, 0x80, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x01, 0xe3, 0x80, 0x00, 0x00, 0x00, 0x01, 0x80, 0x01, 0xe3, 0x80, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x01, 0xe3, 0x80, 0x1c, 0x01, 0xf0, 0x01, 0x80, 0x01, 0xe3, 0x80, 0x3e, 0x07, 0xfc, 0x01, 
0x80, 0x01, 0xfb, 0x80, 0x77, 0x0f, 0xfc, 0x01, 0x80, 0x01, 0xfb, 0x80, 0x63, 0x1c, 0x0c, 0x01, 
0x80, 0x01, 0xfb, 0x80, 0x73, 0x1c, 0x00, 0x01, 0x80, 0x01, 0xfb, 0x80, 0x3e, 0x38, 0x00, 0x01, 
0x80, 0x01, 0xfb, 0x80, 0x1c, 0x38, 0x00, 0x01, 0x80, 0x01, 0xfb, 0x80, 0x00, 0x38, 0x00, 0x01, 
0x80, 0x01, 0xfb, 0xc0, 0x00, 0x38, 0x00, 0x01, 0x80, 0x03, 0xfb, 0xc0, 0x00, 0x38, 0x00, 0x01, 
0x80, 0x03, 0xdb, 0xe0, 0x00, 0x38, 0x00, 0x01, 0x80, 0x07, 0xbf, 0xe0, 0x00, 0x1c, 0x00, 0x01, 
0x80, 0x07, 0xfe, 0xe0, 0x00, 0x1e, 0x0c, 0x01, 0x80, 0x07, 0xfe, 0xe0, 0x00, 0x0f, 0xfc, 0x01, 
0x80, 0x07, 0xfe, 0xe0, 0x00, 0x07, 0xfc, 0x01, 0x80, 0x07, 0xff, 0xe0, 0x00, 0x01, 0xf0, 0x01, 
0x80, 0x07, 0xfd, 0xe0, 0x00, 0x00, 0x00, 0x01, 0x80, 0x03, 0xe7, 0xe0, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x01, 0x80, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
  };

const unsigned char sample_co23 [] PROGMEM = {
 0x00, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x1c, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x0e, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x0c, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x38, 
0x00, 0xc0, 0x00, 0x04, 0x00, 0x00, 0x30, 0x38, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x20, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xe0, 0x00, 0x0c, 0x00, 
0x00, 0x0e, 0x01, 0xff, 0xf8, 0x00, 0x0c, 0x00, 0x00, 0x0e, 0x03, 0xff, 0xfc, 0x00, 0x08, 0x08, 
0x00, 0x0e, 0x07, 0xff, 0xfe, 0x00, 0x00, 0x08, 0x08, 0x04, 0x0f, 0xff, 0xfe, 0x00, 0x00, 0x00, 
0x08, 0x00, 0x1f, 0xff, 0xff, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xe0, 0x00, 
0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xfc, 0x00, 
0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 
0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 
0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 
0x07, 0xff, 0xf0, 0x1f, 0xf0, 0x1f, 0xff, 0x80, 0x07, 0xff, 0xc0, 0x07, 0xc0, 0x07, 0xff, 0xc0, 
0x0f, 0xff, 0x80, 0x07, 0x80, 0x03, 0xff, 0xc0, 0x1f, 0xff, 0x00, 0x07, 0x00, 0x01, 0xff, 0xc0, 
0x1f, 0xfe, 0x03, 0x8f, 0x03, 0x81, 0xff, 0xc0, 0x3f, 0xfe, 0x0f, 0xee, 0x07, 0xc0, 0xff, 0xe0, 
0x3f, 0xfe, 0x0f, 0xfe, 0x0f, 0xe0, 0xff, 0xf0, 0x3f, 0xfc, 0x1f, 0xfe, 0x0f, 0xe0, 0xff, 0xf8, 
0x3f, 0xfc, 0x1f, 0xfe, 0x0f, 0xf0, 0xff, 0xfc, 0x7f, 0xfc, 0x1f, 0xfe, 0x1f, 0xf0, 0xff, 0xfc, 
0x7f, 0xfc, 0x1f, 0xfe, 0x1f, 0xf0, 0xff, 0xfe, 0x7f, 0xfc, 0x1f, 0xfe, 0x0f, 0xf0, 0xff, 0xfe, 
0x7f, 0xfc, 0x1f, 0xfe, 0x0f, 0xe0, 0xff, 0xfe, 0x7f, 0xfc, 0x1f, 0xfe, 0x0f, 0xe0, 0xe1, 0xff, 
0x7f, 0xfe, 0x0f, 0xfe, 0x0f, 0xe0, 0xc0, 0xff, 0x7f, 0xfe, 0x07, 0xcf, 0x07, 0xc1, 0xfc, 0xff, 
0x7f, 0xff, 0x00, 0x07, 0x00, 0x01, 0xfc, 0xff, 0x3f, 0xff, 0x00, 0x07, 0x80, 0x03, 0xf9, 0xff, 
0x3f, 0xff, 0x80, 0x07, 0xc0, 0x07, 0xf1, 0xff, 0x3f, 0xff, 0xe0, 0x0f, 0xe0, 0x0f, 0xe3, 0xff, 
0x3f, 0xff, 0xf8, 0x7f, 0xf8, 0x3f, 0xc3, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xff, 
0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xfe, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 
0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x3f, 0xff, 0x0f, 0xff, 0xc7, 0xff, 0xc0, 
0x00, 0x0f, 0xfc, 0x03, 0xff, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char rh [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1e, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x3c, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x78, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x07, 0x80, 0x00, 0x00, 
0x00, 0x00, 0x03, 0xc0, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x01, 0xe0, 0x00, 0x00, 
0x00, 0x00, 0x07, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0xf0, 0x00, 0x00, 
0x00, 0x00, 0x0e, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x38, 0x00, 0x00, 
0x00, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x1c, 0x00, 0x00, 
0x00, 0x00, 0x78, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x0e, 0x00, 0x00, 
0x00, 0x00, 0xf0, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x07, 0x00, 0x00, 
0x00, 0x00, 0xe0, 0x3c, 0x18, 0x07, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x3e, 0x38, 0x07, 0x80, 0x00, 
0x00, 0x01, 0xc0, 0x7f, 0x38, 0x03, 0x80, 0x00, 0x00, 0x01, 0xc0, 0x77, 0x70, 0x03, 0x80, 0x00, 
0x00, 0x03, 0xc0, 0x77, 0x70, 0x03, 0xc0, 0x00, 0x00, 0x03, 0x80, 0x77, 0xe0, 0x01, 0xc0, 0x00, 
0x00, 0x03, 0x80, 0x7e, 0xe0, 0x01, 0xc0, 0x00, 0x00, 0x03, 0x80, 0x3d, 0xfe, 0x01, 0xc0, 0x00, 
0x00, 0x03, 0x80, 0x03, 0xbf, 0x01, 0xc0, 0x00, 0x00, 0x03, 0x80, 0x03, 0xf7, 0x01, 0xc0, 0x00, 
0x00, 0x03, 0x80, 0x07, 0x77, 0x01, 0xc0, 0x00, 0x00, 0x03, 0x80, 0x07, 0x77, 0x01, 0xc0, 0x00, 
0x00, 0x03, 0x80, 0x0e, 0x7f, 0x01, 0xc0, 0x00, 0x00, 0x03, 0x80, 0x0e, 0x3e, 0x01, 0xc0, 0x00, 
0x00, 0x03, 0xc0, 0x0c, 0x1c, 0x03, 0xc0, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x03, 0x80, 0x00, 
0x00, 0x01, 0xe0, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x07, 0x00, 0x00, 
0x00, 0x00, 0xf0, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x1e, 0x00, 0x00, 
0x00, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x7c, 0x00, 0x00, 
0x00, 0x00, 0x0f, 0xc0, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x1f, 0xe0, 0x00, 0x00, 
0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};



 
void setup() {
  // put your setup code here, to run once:
  pinMode(OLED_SDA, INPUT_PULLUP);           // set pin to input
  pinMode(OLED_SCL, INPUT_PULLUP);           // set pin to input
  Serial.begin(115200);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // display IIITH logo
  display.clearDisplay();
  display.display();
  display.drawBitmap(0, 0,  iiith, 128, 64, WHITE);
  display.display();
   delay(10000);

  // display SCRC in round rect
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.fillRoundRect(0, 2, 128, 64, 8, WHITE);
  display.setTextColor(BLACK);
  display.setTextSize(5);
  display.setCursor(6,10);
  display.println("SCRC");
  display.display();
  delay(10000);

  // display node name in round rect
  display.setCursor(0,0);
  display.fillRoundRect(0, 2, 128, 64, 8, WHITE);
  display.setTextColor(BLACK);
  display.setTextSize(3);
  display.setCursor(6,10);
  display.println("SR-AQ-KH00-00");
  display.display();
  delay(10000);

  // CO2
  display.clearDisplay();
  display.display();
  display.drawBitmap(0, 0,  sample_co23, 64, 64, WHITE);
  display.display();
  
  display.setCursor(70,0);
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.println("552");
  display.setCursor(70,32);
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.println("ppm");
  
  display.display();
  delay(10000);

  // Temperarture
  display.clearDisplay();
  display.display();
  display.drawBitmap(0, 0,  temp, 64, 64, WHITE);
  display.display();
  
  display.setCursor(68,24);
  display.setTextSize(2);
   display.setTextColor(WHITE);
  display.println("23.55");
  display.display();
  delay(10000);

 // RH 
 display.clearDisplay();
  display.display();
  display.drawBitmap(0, 0,  rh, 64, 64, WHITE);
  display.display();
  
  display.setCursor(60,10);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println(" rH");
  
  display.setCursor(60,32);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("55.55");
  display.display();
  delay(10000);
}
void loop() {
  // put your main code here, to run repeatedly:

}
