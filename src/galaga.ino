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

// const int JOYSTICK_UPDOWN_PIN = A1;
// const int JOYSTICK_LEFTRIGHT_PIN = A0;

const int BLASTER_PIN = 8;
const int POT_PIN = A0;

int _lastPress = HIGH;

const int GAME_FIELD_WIDTH = 86;
const int SHIP_Y_POS = 59;
const int LASER_WIDTH = 1;
const int LASER_HEIGHT = 2;

const int MAX_LASERS = 50; // max amount of lasers on the field at a time (default 2)
const int LASER_REMOVED_MARKER = -10; // Y value that signifies the laser will not be removed
const int LASER_SPEED = 2; // default is 8?
int _laser[MAX_LASERS][2]; // array representing the last x and y position of this laser.
int _lasersPresent; // if a laser is already present on the field

void setup() {
  Serial.begin(9600);

  delay(3000);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!_display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  _display.display();  
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  _display.clearDisplay();

  _lasersPresent = 0;

  pinMode(BLASTER_PIN, INPUT_PULLUP);
}

void loop() {
  _display.clearDisplay();

  int moveInput = analogRead(POT_PIN);
  Serial.println(moveInput);
  int shipPosx = (int) ((moveInput / (float) 1023) * GAME_FIELD_WIDTH); // TODO smooth this for no jittering
  // Serial.println(shipPosx);
  drawShip(shipPosx, SHIP_Y_POS);
  
  for (int i = 3; i < 9; i++) {
    drawBug((i * 2) + i, 3);
  }

  int shootbtn = digitalRead(BLASTER_PIN);
  if (_lasersPresent < MAX_LASERS && shootbtn != _lastPress && shootbtn == LOW) {
    // draw new laser at the ship
    Serial.print("new laser at ");
    Serial.println(shipPosx);
    newLaser(shipPosx, SHIP_Y_POS);
  }

  bool readjustLasers = false;

  // if there are lasers present
  if (_lasersPresent > 0) {
    // calculate new positions, redraw laser
    for (int i = 0; i < _lasersPresent; i++) {
      _laser[i][1] -= LASER_SPEED;
      Serial.print("current laser's y: ");
      Serial.println(_laser[i][1]);
      // check if current laser is now above the screen
      if (_laser[i][1] < 0) {
        // mark it as removed
        Serial.print("removed laser ");
        Serial.println(i);
        _laser[i][1] = LASER_REMOVED_MARKER;
        readjustLasers = true;
      } else {
        _display.fillRect(_laser[i][0], _laser[i][1], LASER_WIDTH, LASER_HEIGHT, WHITE);
      }
    }
  }

  if (readjustLasers) {
    // move all lasers down 
    for (int i = 0; i < _lasersPresent-1; i++) {
      if (_laser[i][1] == -10) {
        _laser[i][0] = _laser[i+1][0];
        _laser[i][1] = _laser[i+1][1];
      }
      _lasersPresent--;
    }
  }

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
 * Draws the laserblast at the given coordinate.
 * The coordinate defines the bottom of the blast.
*/
void newLaser(int x, int y) {\
  _lasersPresent++;

  // set position
  // TODO: fix _lasersPresent indexing of lasers
  _laser[_lasersPresent-1][0] = x;
  _laser[_lasersPresent-1][1] = y;
}
