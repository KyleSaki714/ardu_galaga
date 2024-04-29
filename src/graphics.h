#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <Wire.h>
#include "actor.h"

const int LASER_SPEED = 6; // default is 8?

#define SCREEN_WIDTH 128 // OLED _display width, in pixels
#define SCREEN_HEIGHT 64 // OLED _display height, in pixels

#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// Declaration for an SSD1306 _display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
inline Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// BITMAPS
// 'ship', 7x6px
const unsigned char epd_bitmap_ship [] PROGMEM = {
	0x10, 0x10, 0x54, 0x38, 0xba, 0xd6
};
// 'bee', 5x5px
const unsigned char epd_bitmap_bee [] PROGMEM = {
	0xa8, 0x70, 0x20, 0xf8, 0xd8
};
// 'boss_galaga_close', 6x7px
const unsigned char epd_bitmap_boss_galaga_close [] PROGMEM = {
	0x78, 0x30, 0x78, 0xb4, 0x84, 0x48, 0x30
};
// 'boss_galaga_open', 6x7px
const unsigned char epd_bitmap_boss_galaga_open [] PROGMEM = {
	0x78, 0x30, 0x78, 0x78, 0xcc, 0x84, 0x84
};
// 'explode1', 7x7px
const unsigned char epd_bitmap_explode1 [] PROGMEM = {
	0x00, 0x10, 0x10, 0x28, 0x18, 0x00, 0x00
};
// 'explode2', 7x7px
const unsigned char epd_bitmap_explode2 [] PROGMEM = {
	0x00, 0x30, 0x58, 0x04, 0x48, 0x54, 0x10
};
// 'explode3', 7x7px
const unsigned char epd_bitmap_explode3 [] PROGMEM = {
	0x22, 0x84, 0x00, 0x46, 0x00, 0x42, 0x88
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 128)
inline const int epd_bitmap_allArray_LEN = 7;
inline const unsigned char* epd_bitmap_allArray[epd_bitmap_allArray_LEN] = {
	epd_bitmap_bee,
	epd_bitmap_boss_galaga_close,
	epd_bitmap_boss_galaga_open,
	epd_bitmap_ship,
	epd_bitmap_explode1,
	epd_bitmap_explode2,
	epd_bitmap_explode3
};

void checkCollisions(Laser** lasers, int maxLasers,
                     Bee** bees, int maxBees,
                     Ship* ship);

void drawScore(const String scorePrefix, int points);
void drawEnemiesFormation(Bee** bees, int totalAmount, int beeWave);
void drawEnemiesLines(Bee** bees, int totalAmount, int beeWave);
void drawLasers(Laser** lasers, int maxLasers);

#endif