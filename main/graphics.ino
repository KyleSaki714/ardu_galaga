#ifndef GRAPHICS
#define GRAPHICS

#include <Wire.h>
#include <classes.ino>

const int LASER_SPEED = 6; // default is 8?


#define SCREEN_WIDTH 128 // OLED _display width, in pixels
#define SCREEN_HEIGHT 64 // OLED _display height, in pixels

#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// Declaration for an SSD1306 _display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
const int epd_bitmap_allArray_LEN = 7;
const unsigned char* epd_bitmap_allArray[epd_bitmap_allArray_LEN] = {
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
                     Ship* ship) {
  for (int i = 0; i < maxLasers; i++) {
    Laser *currentLaser = lasers[i];
    // TODO loop through galagas too
    for (int j = 0; j < maxBees; j++) {
      Bee *currentBee = bees[j];
      
      // check laser collisions with bee
      bool laserHit = !currentBee->isHidden() && currentLaser->overlaps(*currentBee);
      if (laserHit) {
        // TODO this breaks contract
        currentBee->hide();
        currentBee->hit();
        currentLaser->hide();
      }

      // check ship collision with bee
      bool shipHit = !currentBee->isHidden() && currentBee->overlaps(*ship);
      if (shipHit) {
        // lose life
        ship->hit();
        currentBee->hit();
      }
    }
  }
}

/**
 * Updates the score on screen.
*/
void drawScore(const String scorePrefix, int points) {
  _display.setCursor(0, 5);
  _display.print(scorePrefix);
  _display.print(points);
}

void drawEnemies(Bee** bees, int totalAmount) {
//   _beeMovie = (_beeMovie + 10) % 360;
//   float radian = (_beeMovie * M_PI) / 180;
  for (int j = 0; j < totalAmount; j++) {
    Bee *currentBee = bees[j];
    // currentBee->setX(currentBee->getStartPositionX() + (sin(radian) * 1.1));
    if (currentBee->isDiving()) {
      currentBee->setY(currentBee->getY() + 2);
      if (currentBee->getY() > _display.height()) {
        currentBee->setLocation(currentBee->getStartPositionX(), currentBee->getStartPositionY());
        currentBee->setDive(false);
      }
    }
    currentBee->draw(_display);
  }
}

/**
 * Calculates the drawing of all lasers,
 * checking positions and redrawing.
*/
void drawLasers(Laser** lasers, int maxLasers) {
  // calculate new positions, redraw laser
  for (int i = 0; i < maxLasers; i++) {
    Laser *currentLaser = lasers[i];

    // check if current laser is now above the screen
    if (currentLaser->getY() < 0) {
      // mark it as removed
      currentLaser->hide();
    }

    if (currentLaser->isHidden()) {
      continue;
    }
    int newY = currentLaser->getY() - LASER_SPEED;
    currentLaser->setY(newY);
    currentLaser->draw(_display);
  }
}


#endif