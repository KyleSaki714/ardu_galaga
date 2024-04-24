
#include <Wire.h>
#include <SPI.h>
#include <Shape.hpp>;
#include <ParallaxJoystick.hpp>;

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED _display width, in pixels
#define SCREEN_HEIGHT 64 // OLED _display height, in pixels

// Declaration for an SSD1306 _display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int MAX_ANALOG_VAL = 1023;

const int LED_PIN = 7;

// Ball _playerBall(20, 20, 3);
Rectangle _playerBall(10 ,10, 1, 3);

Rectangle _wall(10 ,10, 4, 40);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!_display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  pinMode(LED_PIN, OUTPUT);

  _display.clearDisplay();
    // Initialize the player
  _playerBall.setDrawFill(true);
  // _playerBall.setCenter(_display.width()/2, _display.height()/2);
}

void loop() {
  // put your main code here, to run repeatedly:
  _display.clearDisplay();

  bool isColliding = _wall.overlaps(_playerBall);

  digitalWrite(LED_PIN, isColliding);

  _wall.draw(_display);

  int moveInput = analogRead(A0);
  // Serial.println(moveInput);
  int shipPosx = (int) ((moveInput / (float) MAX_ANALOG_VAL) * 128);
  
  _playerBall.setLocation(shipPosx, 30);

  _playerBall.draw(_display);

  _display.display();
  delay(10);
}
