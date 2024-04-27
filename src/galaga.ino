/**
 * Kyle Santos
 * Galaga game
 */

#include <Wire.h>
#include <graphics.ino>
#include <Fonts/Picopixel.h>

#define SCREEN_WIDTH 128 // OLED _display width, in pixels
#define SCREEN_HEIGHT 64 // OLED _display height, in pixels

#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// Declaration for an SSD1306 _display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
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


// PINS
// const int JOYSTICK_UPDOWN_PIN = A1;
// const int JOYSTICK_LEFTRIGHT_PIN = A0;
const int BLASTER_PIN = 8;
const int POT_PIN = A0;
int _lastPress = HIGH;

// SIZES AND POSITIONS
// const int GAME_FIELD_WIDTH = 86;
const int GAME_FIELD_WIDTH = _display.width();
// const int SHIP_Y_POS = 59; // Horizontal
// const int SHIP_Y_POS = 122; // Vertical
const int SHIP_Y_POS = 111; // Vertical
const int SHIP_MOVE_MARGIN = 3;
const int SHIP_WIDTH = 7;
const int SHIP_HEIGHT = 6;
const int BEE_WIDTH_AND_HEIGHT = 5;
const int BOSSGALAGA_WIDTH = 6;
const int BOSSGALAGA_HEIGHT = 6;
const int LASER_WIDTH = 1;
const int LASER_HEIGHT = 3;

// LASERS
const int MAX_LASERS = 8; // max amount of lasers on the field at a time (default 2)
// const int LASER_REMOVED_MARKER = 420; // Y value that signifies the current laser slot is not holding a laser
const int LASER_SPEED = 2; // default is 8?
// int _laser[MAX_LASERS][2]; // array representing the last x and y position of this laser.

// GLOBALS
int _points = 0;

Ship _ship(_display.width() / 2, 111, 7, 6);

Laser **_laser;
bool _laserActive[MAX_LASERS];

Actor _bee(32, 32, BEE_WIDTH_AND_HEIGHT, BEE_WIDTH_AND_HEIGHT);
int _beeMovie = 0;

void setup() {
  Serial.begin(9600);
  pinMode(BLASTER_PIN, INPUT_PULLUP);

  delay(3000);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!_display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // IMPORTANT DISPLAY STUFF
  _display.clearDisplay();
  _display.setRotation(1); // ROTATE DISPLAY!!!
  _display.setFont(&Picopixel);

  _display.setTextSize(1);
  _display.setTextColor(WHITE);
  // _display.setCursor(0,5);
  _display.println("Galaga!!!!");
  _display.drawBitmap(30, 30, epd_bitmap_ship, 7, 6, WHITE);
  _display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  _display.clearDisplay();

  // initialize Shapes
  _bee.setSprite(epd_bitmap_bee);
  _bee.setDrawBoundingBox(false);
  _bee.setLocation(32, 32);
  _bee.setExplodeSprites(epd_bitmap_allArray[6]);
  _ship.setSprite(epd_bitmap_ship);
  _ship.setDrawBoundingBox(false);

  // initialize lasers
  _laser = new Laser*[MAX_LASERS];
  for (int i = 0; i < MAX_LASERS; i++) {
    _laserActive[i] = false;
    _laser[i] = new Laser(_display.width() / 2, _display.height() / 2, LASER_WIDTH, LASER_HEIGHT);
    _laser[i]->setDrawBoundingBox(true);
    _laser[i]->setDrawSprite(false);
  }

  updateScore(0);
}

void loop() {
  _display.clearDisplay();

  drawEnemies();

  updateScore(_points);

  int moveInput = analogRead(POT_PIN);
  int shipPosx = (int) ((moveInput / (float) 1023) * GAME_FIELD_WIDTH); // TODO smooth this for no jittering

  // constrain ship movement
  if (shipPosx < SHIP_MOVE_MARGIN) {
    shipPosx = SHIP_MOVE_MARGIN;
  }
  if (shipPosx > _display.width() - SHIP_MOVE_MARGIN) {
    shipPosx = _display.width() - SHIP_MOVE_MARGIN;
  }
  _ship.setLocation(shipPosx, SHIP_Y_POS);
  _ship.forceInside(0, 110, 64 + SHIP_MOVE_MARGIN, 8);
  _ship.draw(_display);

  int shootbtn = digitalRead(BLASTER_PIN);
  if (shootbtn != _lastPress && shootbtn == LOW) {
    newLaser(shipPosx, SHIP_Y_POS);
  }

  drawLasers();

  _lastPress = shootbtn;

  _display.display();
  delay(10);
}

void updateScore(int points) {
  String score = "Score: ";
  // int16_t x1, y1;
  // uint16_t w, h;
  // _display.getTextBounds(score, 0, 0, &x1, &y1, &w, &h);
  // _display.setCursor(0,y1);
  // _display.print("High Score: ");
  // _display.println(420420);
  // _display.setCursor(0,8);
  _display.setCursor(0, 5);
  _display.print(score);
  _display.print(points);
}

void drawEnemies() {
  _beeMovie = (_beeMovie + 30) % 360;
  float radian = (_beeMovie * M_PI) / 180;
  // Serial.println(radian);
  _bee.setX((_display.width() / 2) + (sin(radian) * 2));
  // bool isColliding = _bee.overlaps()

  for (int i = 0; i < MAX_LASERS; i++) {
    Laser *currentLaser = _laser[i];
    bool hit = currentLaser->overlaps(_bee);
    if (hit) {
      _bee.explode();
      // todo laser hit
    }
  }

  _bee.draw(_display);
}

/**
 * Draws a bug at the given coordinate.
 * The coordinate defines the top left of the bug.
*/
void drawBug(int x, int y) {
  _display.fillRect(x, y, 2, 2, WHITE);
}

/**
 * Calculates the drawing of all lasers,
 * checking positions and redrawing.
*/
void drawLasers() {
  // calculate new positions, redraw laser
  for (int i = 0; i < MAX_LASERS; i++) {
    Laser *currentLaser = _laser[i];

    // check if current laser is now above the screen
    if (currentLaser->getY() < 0) {
      // mark it as removed
      _laserActive[i] = false;
    }

    if (_laserActive[i] == false) {
      continue;
    }
    int newY = currentLaser->getY() - LASER_SPEED;
    currentLaser->setY(newY);
    currentLaser->draw(_display);
  }
}

/**
 * Returns the index of a free laser slot.
 * returns -1 if no slots are available;
*/
int findOpenLaserSlot() {
  for (int i = 0; i < MAX_LASERS; i++) {
    if (_laserActive[i] == false) {
      return i;
    }
  }
  return -1;
}

/**
 * Establishes the laser at the given coordinate.
 * The coordinate defines the bottom of the blast.
 * Returns 0 on success: the laser is established
 *         1 on failure.
*/
int newLaser(int x, int y) {
  int newLaserIndex = findOpenLaserSlot();
  if (newLaserIndex != -1) {
    Laser *pew = (_laser[newLaserIndex]);
    pew->setLocation(x, y);
    _laserActive[newLaserIndex] = true;
    return 0;
  }
  return 1;
}
