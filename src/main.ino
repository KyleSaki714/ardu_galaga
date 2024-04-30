/**
 * Kyle Santos
 * Galaga game
 */
#include <SPI.h>
#include <Wire.h>
// #include "graphics.h"
#include "gamelogic.h"
// #include <audio.ino>

typedef enum {
  MENU,
  GAME,
  LOSE
} GAMESTATE;

GAMESTATE _currentState;

/**
 * Facilitates state transitions, calls necessary setup functions
 * for the next state.
*/
void changeState(GAMESTATE newState) {
  switch (newState)
  {
  case MENU:
    // _display.println("Loading Menu...");
    // _display.display();
    delay(1000);
    break;
  case GAME:
    _display.println("Loading Game...");
    _display.display();
    initializeActors();
    drawScore(SCORE, _currentScore);
    delay(2000);
    break;
  case LOSE:
    // _display.println("Loading LOSE...");
    // _display.display();
    delay(1500);
  default:
    break;
  }

  _currentState = newState;
}

void setup() {
  Serial.begin(9600);
  pinMode(BLASTER_PIN, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);

  delay(2000);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!_display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  Serial.println("Display Good");

  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    for(;;);
  }
  Serial.println("LIS3DH Good");

  // IMPORTANT DISPLAY STUFF
  _display.clearDisplay();
  _display.setRotation(1); // ROTATE DISPLAY!!!
  _display.setFont(&Picopixel);

  _display.setTextSize(1);
  _display.setTextColor(WHITE);
  // _display.setCursor(0,5);
  _display.print("Getting ready");
  // _display.drawBitmap(30, 30, epd_bitmap_ship, 7, 6, WHITE);
  _display.display();
  delay(250); // Pause for 2 seconds
  _display.print(".");
  _display.display();
  delay(250);
  _display.print(".");
  _display.display();
  delay(250);
  _display.println(".");
  _display.display();
  delay(500);
  Serial.println("Loading Game..");

  // Clear the buffer
  _display.clearDisplay();

  changeState(MENU);
}

void loop() {
  _display.clearDisplay();

  switch (_currentState)
  {
  case MENU:
    menu();
    break;
  case GAME:
    game();
    break;
  case LOSE:
    lose();
  default:
    break;
  }

  _display.display();
  delay(10);
}

void menu() {
  // Serial.println("menu");
  _display.setCursor(0, 5);
  _display.println("FLY DESTROYER");
  _display.println("Press FIRE to start");

  // lis.read();
  // Serial.print("lis Y:  "); Serial.println(lis.y);

  // lis.read();      // get X Y and Z data at once
  // // Then print out the raw data
  // Serial.print("X:  "); Serial.print(lis.x);
  // Serial.print("  \tY:  "); Serial.print(lis.y);
  // Serial.print("  \tZ:  "); Serial.print(lis.z);

  sensors_event_t event;
  lis.getEvent(&event);

  // /* Display the results (acceleration is measured in m/s^2) */
  // Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
  // Serial.print(" \tY: "); Serial.print(event.acceleration.y);
  // Serial.print(" \tZ: "); Serial.print(event.acceleration.z);
  // Serial.println(" m/s^2 ");

  // // int pot = analogRead(POT_PIN);
  // // float thresh =((pot / (float) 1023) * 2.0);

  // // Serial.println(event.gyro.y);

  // ship is upright
  if (event.acceleration.z > 5) {
    int moveAccel = map(event.acceleration.y, -9, 9, 0, 64);
    _shipMove = moveAccel;
    // if (_shipMove < 0) {
    //   _shipMove = 0;
    // }
    // if (_shipMove > _display.width()) {
    //   _shipMove = _display.width();
    // }
  }

  Serial.println(_shipMove);

  // if (event.acceleration.z > 5) {
    // Serial.println("upright");



    // if change in y direction
    // float vel = abs(event.acceleration.y - _lisPrevYaccel);
    // if (vel > MOVE_FIGHTER_THRESH) {
    // // if (vel > thresh) {
    //   // Serial.print("thresh value: ");
    //   // Serial.print(thresh);
    //   // Serial.println(" MOVE!!");
    //   // Serial.print("y: "); Serial.print(event.acceleration.y);
    //   // Serial.print(" prev: "); Serial.println(_lisPrevYaccel);

    //   // if current is larger than previous, going right otherwise left
    //   int direction = event.acceleration.y > _lisPrevYaccel ? 1 : -1;
    //   _shipMove = (_shipMove + direction) % _display.width();
    //   // Serial.println(_shipMove);
    //   // if (event.acceleration.y > _lisPrevYaccel) {
    //   //   Serial.println("right");
    //   // } else {
    //   //   Serial.println("left");
    //   // }

    // }
  // }

  _lisPrevYaccel = event.acceleration.y;

  int btnPress = digitalRead(BLASTER_PIN);
  if (btnPress != _lastPress && btnPress == LOW) {
    changeState(GAME);
  }


}

void game() {
  // Serial.println("game");

  if (_currentEnemyRoutine == FORMATION) {
    drawEnemiesFormation(_bee, MAX_BEES, _beeMovie);
    _display.println("FORMATION");
  } else if (_currentEnemyRoutine == SCATTER) {
    drawEnemiesScatter(_bee, MAX_BEES, _beeMovie);
    _display.println("SCATTER");
  }


  int gameStatus = gameLoop();

  if (gameStatus) {
    changeState(LOSE);
  }

  drawScore(SCORE, _currentScore);

  _display.print(" p: ");
  _display.println(_enemiesKilledTotal);

  drawLasers(_laser, MAX_LASERS);

  _beeMovie = (_beeMovie + 10) % 360;
}

void lose() {
  Serial.println("lose");
  _display.setCursor(0, 5);
  _display.println("YOU LOST");

  _display.println("FINAL SCORE: ");
  _display.println(_currentScore);

  int btnPress = digitalRead(BLASTER_PIN);
  if (btnPress != _lastPress && btnPress == LOW) {
    changeState(MENU);
  }


}