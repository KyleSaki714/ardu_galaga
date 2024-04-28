#ifndef GAMELOGIC_H_
#define GAMELOGIC_H_

#include "Arduino.h"
// #include "actor.h"
#include "graphics.h"

// INPUT
// TODO: make input class
// const int JOYSTICK_UPDOWN_PIN = A1;
// const int JOYSTICK_LEFTRIGHT_PIN = A0;
const int BLASTER_PIN = 9;
const int POT_PIN = A0;
int _lastPress = HIGH;

// CONST EXPRESSIONS
const int MAX_LASERS = 2; // max amount of lasers on the field at a time (default 2)

// CONST STRINGS
const String BEE = "Bee";
const String BOSSGALAGA = "BossGalaga";
const String SCORE = "Score: ";

// GLOBALS
int _currentScore = 0;
Ship* _ship;
Laser **_laser;
Bee **_bee;
const int MAX_BEES = 32;
// bool _formationPositions_bees[MAX_BEES];
// int formationPositions_galaga[4];
int _beeMovie = 0;
bool _lostGame;

void initializeActors();
void loseGame();
void updateScore(int);
void updateScore(String);
void checkHits();
int findOpenLaser();
void newLaser(int, int);
void randomDive();
int gameLoop();

#endif
