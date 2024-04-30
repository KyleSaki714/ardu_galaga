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
    _display.println("Loading...");
    _display.display();
    initializeActors();
    drawScore(SCORE, _currentScore);
    delay(2000);
    break;
  case LOSE:
    _display.println("BETTER LUCK NEXT TIME");
    delay(500);
    digitalWrite(VIBROMOTOR_OUTPUT_PIN, LOW);
    _display.display();
    delay(1500);
  default:
    break;
  }

  _currentState = newState;
}

void setup() {
  Serial.begin(9600);
  pinMode(BLASTER_PIN, INPUT_PULLUP);
  // pinMode(10, INPUT_PULLUP);
  pinMode(A1, INPUT);
  pinMode(5, OUTPUT);

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

  // sensors_eve/t(&event);

  // /* Display the results (acceleration is measured in m/s^2) */
  // Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
  // Serial.print(" \tY: "); Serial.print(event.acceleration.y);
  // Serial.print(" \tZ: "); Serial.print(event.acceleration.z);
  // Serial.println(" m/s^2 ");

  int btnPress = digitalRead(BLASTER_PIN);
  if (btnPress != _lastPress && btnPress == LOW) {
    changeState(GAME);
  }

}

void game() {
  // Serial.println("game");

  if (_currentEnemyRoutine == FORMATION) {
    drawEnemiesFormation(_bee, MAX_BEES, _beeMovie);
    // _display.println("FORMATION");
  } else if (_currentEnemyRoutine == SCATTER) {
    drawEnemiesScatter(_bee, MAX_BEES, _beeMovie);
    // _display.println("SCATTER");
  }


  int gameStatus = gameLoop();

  if (gameStatus) {
    digitalWrite(VIBROMOTOR_OUTPUT_PIN, HIGH);
    changeState(LOSE);
  }

  drawScore(SCORE, _currentScore);

  _display.print(" ded: ");
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