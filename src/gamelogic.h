#ifndef GAMELOGIC_H_
#define GAMELOGIC_H_

// #include "Arduino.h"
// #include "actor.h"
#include "graphics.h"
#include "audio.hpp"

// INPUT
// TODO: make input class
// const int JOYSTICK_UPDOWN_PIN = A1;
// const int JOYSTICK_LEFTRIGHT_PIN = A0;
const int BLASTER_PIN = 9;
const int POT_PIN = A0;
inline int _lastPress = HIGH;

// CONST EXPRESSIONS
const int MAX_LASERS = 2; // max amount of lasers on the field at a time (default 2)

// CONST STRINGS
const String BEE = "Bee";
const String BOSSGALAGA = "BossGalaga";
const String SCORE = "Score: ";

// GLOBALS
inline int _currentScore = 0;
inline Ship* _ship;
inline Laser **_laser;
inline Bee **_bee;
const int MAX_BEES = 24;
// bool _formationPositions_bees[MAX_BEES];
// int formationPositions_galaga[4];
inline int _beeMovie = 0;
inline bool _lostGame;
inline int _shotsFired = 0;
inline int _enemiesKilledTotal = 0;
inline int _enemiesKilledSinceLastPhase = 0; // reset every phase

typedef enum ENEMYROUTINE {
  FORMATION,
  SCATTER
} ENEMYROUTINE;

inline ENEMYROUTINE _currentEnemyRoutine;

void initializeActors();
void loseGame();
void updateKillCount();
void updateScore(int);
void updateScore(String);
void checkHits();
int findOpenLaser();
void newLaser(int, int);
void randomEnemyDive();
int gameLoop();

#endif
