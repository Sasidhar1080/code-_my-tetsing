#include <Adafruit_SH1106.h>
#include <Adafruit_GFX.h>


//define front,back,count varible
int F = 25;
int B = 26;
int count = 0;

//define oled pins
#define OLED_SDA 21
#define OLED_SCL 22

//ardfruit commands
Adafruit_SH1106 display(21, 22);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SH1106.h!");
#endif

//Define bit map of SCRC logo and IIITH logo
const unsigned char bitmap_scrc [] PROGMEM = {
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x04, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x04, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x04, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xc4, 0x00, 0x1c, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xc4, 0x00, 0x1c, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xc4, 0xff, 0x10, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x08, 0xc4, 0xff, 0x10, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xde, 0x08, 0xc4, 0x00, 0x11, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xde, 0x08, 0x04, 0x00, 0x13, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0x9e, 0x08, 0x04, 0xff, 0x13, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0x9e, 0x08, 0x04, 0xff, 0x10, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0x9e, 0x08, 0xc4, 0x00, 0x10, 0x0f, 0x67, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0x9e, 0x08, 0xc4, 0x00, 0x10, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0x9e, 0x08, 0xc4, 0xff, 0x11, 0x0f, 0x77, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0x82, 0x08, 0xc4, 0xff, 0x13, 0x8f, 0xaf, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xfe, 0x10, 0x08, 0xc4, 0x00, 0x13, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xf8, 0x5c, 0x08, 0x04, 0x00, 0x10, 0x08, 0x20, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xf3, 0xde, 0x08, 0x04, 0xff, 0x10, 0x09, 0xd9, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xde, 0x08, 0xc4, 0x7f, 0x10, 0x0f, 0xdf, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xde, 0x08, 0xc4, 0x00, 0x10, 0x0f, 0xdf, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0x9e, 0x08, 0xc4, 0x7e, 0x13, 0x8f, 0xdf, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xde, 0x08, 0xc4, 0xff, 0x13, 0xcf, 0xdf, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0x8e, 0x08, 0xc4, 0x00, 0x11, 0x8f, 0xdf, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0x8e, 0x08, 0x04, 0x00, 0x10, 0x0f, 0xdf, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xfd, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0f, 0x07, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xe0, 0x0f, 0x1f, 0xf9, 0xff, 0x3f, 0xc0, 0x0f, 0x00, 0x0f, 0x00, 0x79, 0xc0, 0x03, 0x3f, 0xcf,
0xc0, 0x0f, 0x0f, 0xf0, 0xfe, 0x1f, 0xc0, 0x06, 0x00, 0x06, 0x00, 0x31, 0x80, 0x03, 0x1f, 0x87,
0x87, 0xff, 0x07, 0xe0, 0xfc, 0x0f, 0xc7, 0xe3, 0xf0, 0xfc, 0x7f, 0xf1, 0xfc, 0x7f, 0x8f, 0x0f,
0x8f, 0xff, 0x07, 0xe0, 0xfc, 0x0f, 0xc7, 0xe3, 0xf8, 0xfc, 0x7f, 0xf1, 0xfc, 0x7f, 0xc6, 0x1f,
0x87, 0xff, 0x03, 0xc0, 0xf8, 0xc7, 0xc7, 0xe3, 0xf8, 0xfc, 0x7f, 0xf1, 0xfc, 0x7f, 0xc2, 0x3f,
0xc0, 0x07, 0x03, 0xc0, 0xf8, 0xc7, 0xc0, 0x03, 0xf8, 0xfc, 0x7f, 0xf1, 0xfc, 0x7f, 0xe0, 0x7f,
0xff, 0xc7, 0x11, 0x88, 0xf1, 0xe3, 0xc0, 0x07, 0xf8, 0xfc, 0x7f, 0xf1, 0xfc, 0x7f, 0xf0, 0xff,
0xff, 0xc7, 0x11, 0x18, 0xe0, 0x03, 0xc7, 0x0f, 0xf8, 0xfc, 0x7f, 0xf1, 0xfc, 0x7f, 0xf8, 0xff,
0xff, 0xc7, 0x18, 0x18, 0xe0, 0x01, 0xc7, 0x8f, 0xf8, 0xfc, 0x7f, 0xf1, 0xfc, 0x7f, 0xf8, 0xff,
0xc0, 0x07, 0x1c, 0x38, 0xc7, 0xf1, 0xc7, 0xc7, 0xf8, 0xfc, 0x00, 0x71, 0xfc, 0x7f, 0xf8, 0xff,
0x80, 0x07, 0x1c, 0x38, 0xc7, 0xf8, 0xc7, 0xe3, 0xf9, 0xfe, 0x00, 0x31, 0xfc, 0x7f, 0xf8, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xcf, 0xcf, 0xe7, 0xe7, 0xff, 0xf1, 0xfc, 0xfd, 0xbf, 0xff, 0x3f, 0x1f, 0xf7, 0xc7, 0xe7, 0xe7,
0x8f, 0xcf, 0xcf, 0xef, 0xf3, 0xf1, 0xfb, 0xfc, 0xbf, 0xfe, 0xff, 0x3f, 0x37, 0xef, 0xef, 0xe3,
0x8f, 0xcf, 0xe7, 0xe7, 0xe3, 0xf1, 0xf3, 0xfc, 0x3f, 0xfc, 0xff, 0x3f, 0x07, 0xef, 0xe7, 0xe7,
0x8f, 0xcf, 0xd7, 0xef, 0xe9, 0xf1, 0xf9, 0xfd, 0xbf, 0xfe, 0x7f, 0x3f, 0x67, 0xef, 0xef, 0xe3,
0xff, 0xff, 0xef, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff

};

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
void setup() {
  
pinMode(F,INPUT);
pinMode(B,INPUT);
pinMode(OLED_SDA, INPUT_PULLUP);           // set pin to input
pinMode(OLED_SCL, INPUT_PULLUP);           // set pin to input
Serial.begin(9600);
}

void loop() {

display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
display.display();
delay(2000);

// count logic 
Serial.print("f = ");
Serial.println(analogRead(F));
Serial.print("b = ");
Serial.println(analogRead(B));

//Enter Count 
    if(analogRead(F)<1010)
    {
      for(int i=0;i<300;i++)
       {
          if(analogRead(B)<1010)
            {
              count++;
              Serial.println(count);
              break;
            }
        delay(1);
       }
    }
//Exit Count 
  else if(analogRead(B)<1010)
    {
      for(int i=0;i<300;i++)
       {
          if(analogRead(F)<1010)
            {
              count--;
              Serial.println(count);
              break;
            }
        delay(1);
       }
    }
////iiiit logo 
//display.clearDisplay();
//display.display();
//display.drawBitmap(0, 0,  iiith, 128, 64, WHITE);
//display.display();
//delay(1000);
//
//
//// display scrc logo
//display.clearDisplay();
//display.drawBitmap(0, 0, bitmap_scrc, 128, 64, WHITE);
//display.display();
//delay(1000);

//Display visitor count
display.clearDisplay();
display.setCursor(0,0);
display.fillRoundRect(0, 2, 128, 64, 8, WHITE);

display.setCursor(30,10);
display.setTextSize(2.5);
display.setTextColor(BLACK);
display.println("COUNT:");
     
display.setCursor(50,40);
display.setTextSize(2);
display.setTextColor(BLACK);
display.println(count);
display.display();

  
}
