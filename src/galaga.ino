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

void setup() {
  Serial.begin(9600);
  pinMode(BLASTER_PIN, INPUT_PULLUP);

  delay(3000);

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

  // _display.setTextSize(1);
  // _display.setTextColor(WHITE);
  // // _display.setCursor(0,5);
  // _display.println("Galaga!!!!");
  // // _display.drawBitmap(30, 30, epd_bitmap_ship, 7, 6, WHITE);
  // _display.display();
  // delay(2000); // Pause for 2 seconds
  Serial.println("Loading Game..");

  // Clear the buffer
  _display.clearDisplay();


  initializeActors();

  drawScore(SCORE, _currentScore);
}

void loop() {
  _display.clearDisplay();



  _display.display();
  delay(10);
}

void stateTransition(GAMESTATES state) {
  switch (state) {

  }
}

void menu() {

}

void game() {
  drawEnemies(_bee, MAX_BEES);

  gameLoop();

  drawScore(SCORE, _currentScore);

  drawLasers(_laser, MAX_LASERS);
}

void lose() {

}

