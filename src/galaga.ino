/**
 * Kyle Santos
 * Galaga game
 */
#include <SPI.h>
#include <Wire.h>
#include <graphics.ino>
#include <gamelogic.ino>
#include <audio.ino>
#include <Fonts/Picopixel.h>

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
    _display.println("Loading Menu...");
    _display.display();
    delay(2000);
    break;
  case GAME:
    _display.println("Loading Game...");
    _display.display();
    initializeActors();
    drawScore(SCORE, _currentScore);
    delay(2000);
    break;
  case LOSE:
  default:
    break;
  }

  _currentState = newState;
}

void setup() {
  Serial.begin(9600);
  pinMode(BLASTER_PIN, INPUT_PULLUP);

  delay(2000);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!_display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  Serial.println("Display Good");

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
  delay(500); // Pause for 2 seconds
  _display.print(".");
  _display.display();
  delay(500);
  _display.print(".");
  _display.display();
  delay(500);
  _display.println(".");
  _display.display();
  delay(1000);
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
  Serial.println("menu");
  _display.println("Return of the Pelicans");
  _display.println("Press FIRE to start");


  int btnPress = digitalRead(BLASTER_PIN);
  if (btnPress != _lastPress && btnPress == LOW) {
    changeState(GAME);
  }


}

void game() {
  Serial.println("game");

  drawEnemies(_bee, MAX_BEES);

  gameLoop();

  drawScore(SCORE, _currentScore);

  drawLasers(_laser, MAX_LASERS);
}

void lose() {
  Serial.println("lose");
  _display.println("YOU LOST");

  _display.println("FINAL SCORE: ");
  _display.println(_currentScore);

  int btnPress = digitalRead(BLASTER_PIN);
  if (btnPress != _lastPress && btnPress == LOW) {
    changeState(MENU);
  }


}

