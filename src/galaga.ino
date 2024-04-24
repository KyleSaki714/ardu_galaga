/**
 * Kyle Santos
 * Galaga game
 */

#include <Wire.h>
#include <SPI.h>
#include <Shape.hpp>;
#include <ParallaxJoystick.hpp>;

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED _display width, in pixels
#define SCREEN_HEIGHT 64 // OLED _display height, in pixels

#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// Declaration for an SSD1306 _display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// PINS
// const int JOYSTICK_UPDOWN_PIN = A1;
// const int JOYSTICK_LEFTRIGHT_PIN = A0;
const int BLASTER_PIN = 8;
const int POT_PIN = A0;
int _lastPress = HIGH;

// SIZES AND POSITIONS
const int GAME_FIELD_WIDTH = 86;
// const int SHIP_Y_POS = 59; // Horizontal
const int SHIP_Y_POS = 122; // Vertical
const int SHIP_WIDTH = 7;
const int SHIP_HEIGHT = 6;
const int BEE_WIDTH_AND_HEIGHT = 5;
const int BOSSGALAGA_WIDTH = 6;
const int BOSSGALAGA_HEIGHT = 6;
const int LASER_WIDTH = 1;
const int LASER_HEIGHT = 3;

// LASERS
const int MAX_LASERS = 2; // max amount of lasers on the field at a time (default 2)
const int LASER_REMOVED_MARKER = 420; // Y value that signifies the current laser slot is not holding a laser
const int LASER_SPEED = 2; // default is 8?
int _laser[MAX_LASERS][2]; // array representing the last x and y position of this laser.

Rectangle _bee(32, 32, BEE_WIDTH_AND_HEIGHT, BEE_WIDTH_AND_HEIGHT);
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

  _display.clearDisplay();
  _display.setRotation(1);
  _display.setTextSize(1);
  _display.setTextColor(WHITE);
  _display.setCursor(0,0);
  _display.println("Galaga!!!!");
  _display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  _display.clearDisplay();

  _bee.setLocation(32, 32);

  for (int i = 0; i < MAX_LASERS; i++) {
    _laser[i][0] = 0;
    _laser[i][1] = LASER_REMOVED_MARKER;
  }

}

void loop() {
  _display.clearDisplay();


  _beeMovie = (_beeMovie + 1) % 360;
  float radian = (_beeMovie * 71) / 4068;
  Serial.println(radian);
  _bee.setX(_bee.getX() + (sin(radian)));
  // bool isColliding = _bee.overlaps()
  _bee.draw(_display);

  int moveInput = analogRead(POT_PIN);
  // Serial.println(moveInput);
  int shipPosx = (int) ((moveInput / (float) 1023) * GAME_FIELD_WIDTH); // TODO smooth this for no jittering
  // Serial.println(shipPosx);
  drawShip(shipPosx, SHIP_Y_POS);

  // for (int i = 3; i < 9; i++) {
  //   drawBug((i * 2) + i, 3);
  // }

  int shootbtn = digitalRead(BLASTER_PIN);
  if (shootbtn != _lastPress && shootbtn == LOW) {
    // draw new laser at the ship
    newLaser(shipPosx, SHIP_Y_POS);
  }

  drawLasers();

  _lastPress = shootbtn;

  _display.display();
  delay(10);
}

/**
 * Draws the ship at the given coordinate.
 * The coordinate defines the top middle of the ship.
*/
void drawShip(int x, int y) {
  _display.fillTriangle(x, y, x-2, y+5, x+2, y+5, WHITE);
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
    // check if current laser is now above the screen
    if (_laser[i][1] < 0) {
      // mark it as removed
      _laser[i][0] = 0;
      _laser[i][1] = LASER_REMOVED_MARKER;
    }

    if (_laser[i][1] == LASER_REMOVED_MARKER) {
      continue;
    }

    _laser[i][1] -= LASER_SPEED;

    _display.fillRect(_laser[i][0], _laser[i][1], LASER_WIDTH, LASER_HEIGHT, WHITE);
  }
}

/**
 * Returns the index of a free laser slot.
 * returns LASER_REMOVED_MARKER if no slots are available;
*/
int findOpenLaserSlot() {
  for (int i = 0; i < MAX_LASERS; i++) {
    if (_laser[i][1] == LASER_REMOVED_MARKER) {
      return i;
    }
  }
  return LASER_REMOVED_MARKER;
}

/**
 * Establishes the laser at the given coordinate.
 * The coordinate defines the bottom of the blast.
 * Returns 0 on success: the laser is established
 *         1 on failure.
*/
int newLaser(int x, int y) {
  int newLaserIndex = findOpenLaserSlot();
  if (newLaserIndex != LASER_REMOVED_MARKER) {
    // set position
    _laser[newLaserIndex][0] = x;
    _laser[newLaserIndex][1] = y;
    return 0;
  }
  return 1;
}
