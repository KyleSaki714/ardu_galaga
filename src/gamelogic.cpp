#include "gamelogic.h"

// const int GAME_FIELD_WIDTH = 86;
// const int GAME_FIELD_WIDTH = _display.width();

// SIZES AND POSITIONS
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

// const int LASER_REMOVED_MARKER = 420; // Y value that signifies the current laser slot is not holding a laser
// int _laser[MAX_LASERS][2]; // array representing the last x and y position of this laser.

const int DIVE_INTERVAL = 1500; // no enemy must dive within this interval from the last dive.
int _lastDive = 0;

Audio* audio;

/**
 * Called in setup.
*/
void initializeActors() {

  _currentEnemyRoutine = FORMATION;

  audio = new Audio(6);

  randomSeed(analogRead(5));
  _lostGame = false;
  _currentScore = 0;
  _enemiesKilledSinceLastPhase = 0;
  _enemiesKilledTotal = 0;


  Serial.println("Initializing Actors...");

    // initialize Ship
  _ship = new Ship(0, 111, 7, 6);
  _ship->recover();
  _ship->setSprite(epd_bitmap_ship);
  _ship->setDrawBoundingBox(false);
  _ship->setDrawCenterW(true);

  int FORMATION_X_START = 2;
  int FORMATION_X_END = 57;
  int FORMATION_Y_START = 35;
  int FORMATION_Y_END = 53;

  // initialize Bees
  int beex = FORMATION_X_START;
  int beey = FORMATION_Y_START;
  _bee = new Bee*[MAX_BEES];
  // int formationPos = 0;
  // TODO: fix the last bee
  Serial.println("Initializing Bees...");
  for (int i = 0; i < MAX_BEES; i++) {
    _bee[i] = new Bee(beex, beey, BEE_WIDTH_AND_HEIGHT, BEE_WIDTH_AND_HEIGHT);
  //   // formationPos++;
  //   Serial.print("new bee (");
  //   Serial.print(i);
  //   Serial.print(") at ");
  //   Serial.print(beex);
  //   Serial.print(", ");
  //   Serial.print(beey);
  //   Serial.println(".");
  //   _bee[i]->setSprite(epd_bitmap_bee);
  //   _bee[i]->setDrawBoundingBox(false);

    if (i != 0 && i % 8 == 0) {
      beex = FORMATION_X_START;
      beey = beey + 6;
    } else {
      // formationPos++;
      beex = beex + 8;
    }
  }
  initializeFormation(_bee, MAX_BEES);
  Serial.println(" Bees done");

  // initialize lasers
  Serial.print("Initializing Lasers...");
  _laser = new Laser*[MAX_LASERS];
  for (int i = 0; i < MAX_LASERS; i++) {
    _laser[i] = new Laser(-OFFSCREEN_COORDS, -OFFSCREEN_COORDS, LASER_WIDTH, LASER_HEIGHT);
    _laser[i]->hide();
    _laser[i]->setDrawBoundingBox(true);
    _laser[i]->setDrawSprite(false);
  }
  Serial.println(" Lasers done. Actors Initialized");
}

void loseGame() {
  _lostGame = true;
}

void updateKillCount() {
  _enemiesKilledTotal++;
  _enemiesKilledSinceLastPhase++;
}

/**
 * ADDS given score amount to the points total.
*/
void updateScore(int newScore) {
  _currentScore += newScore;
}

/**
 * Given the enemy name, adds corresponding amount to the points total.
*/
void updateScore(String enemyName) {
  String name = enemyName;
  Serial.print("Hit ");
  Serial.println(name);
  if (name == BEE) {
    updateScore(50);
  } else if (name == BOSSGALAGA) {
    updateScore(150);
  }
}

/**
 * Checks the enemies after having been hit and updates points accordingly.
*/
void checkHits() {
  if (_ship->wasJustHit()) {
    loseGame();
  }

  for (int j = 0; j < MAX_BEES; j++) {
    Bee *currentBee = _bee[j];
    if (currentBee->wasJustHit()) {
      updateScore(BEE);
      updateKillCount();
      currentBee->recover();
    }
  }
}

/**
 * Finds an open laser to shoot.
 * Returns -1 if no lasers are available.
*/
int findOpenLaser() {
  for (int i = 0; i < MAX_LASERS; i++) {
    Laser *pew = (_laser[i]);
    if (pew->isHidden()) {
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
void newLaser(int x, int y) {
  int newLaserIndex = findOpenLaser();
  Serial.print("Hi, this is Laser ");
  Serial.println(newLaserIndex);
  if (newLaserIndex != -1) {
    Laser *pew = (_laser[newLaserIndex]);
    pew->setLocation(x, y);
    pew->show();
    audio->setBlasterShot();
  }
}

/**
 * Selects one alive enemy to dive.
*/
void randomEnemyDive() {
  // 3 / 100 chance per frame
  long randLong = random(100);
  if (randLong > 3) {
    return;
  }

  // choose one of the alive bees
  randLong = random(MAX_BEES);
  Bee* b =_bee[randLong];
  if (!b->isHidden() && !b->isDiving() && (millis() - _lastDive) > DIVE_INTERVAL) {
    _bee[randLong]->startDive(_ship->getX(), _ship->getY());
    audio->setEnemyDive();
    _lastDive = millis();
  }
}


/**
 * returns 1 if the game is lost.
*/
int gameLoop() {

  checkCollisions(_laser, MAX_LASERS, _bee, MAX_BEES, _ship);
  checkHits();

  if (_currentEnemyRoutine == FORMATION) {
    if (_enemiesKilledSinceLastPhase > 20) {
      _currentEnemyRoutine = SCATTER;
      _enemiesKilledSinceLastPhase = 0;
      initializeScatter(_bee, MAX_BEES);
    }
    randomEnemyDive();
  } else if (_currentEnemyRoutine == SCATTER) {
    if (_enemiesKilledSinceLastPhase > 50) {
      _currentEnemyRoutine = FORMATION;
      _enemiesKilledSinceLastPhase = 0;
      initializeFormation(_bee, MAX_BEES);
    }
    
  }

  // move ship
  int moveInput = analogRead(POT_PIN);
  int shipPosx = (int) ((moveInput / (float) 1023) * 64); // TODO smooth this for no jittering

  // constrain ship movement
  if (shipPosx < SHIP_MOVE_MARGIN) {
    shipPosx = SHIP_MOVE_MARGIN;
  }
  if (shipPosx > _display.width() - SHIP_MOVE_MARGIN) {
    shipPosx = _display.width() - SHIP_MOVE_MARGIN;
  }
  _ship->setLocation(shipPosx, SHIP_Y_POS);
  _ship->forceInside(0, 110, 64 + SHIP_MOVE_MARGIN, 8);
  _ship->draw(_display);

  int shootbtn = digitalRead(BLASTER_PIN);
  if (shootbtn != _lastPress && shootbtn == LOW) {
    newLaser(shipPosx, SHIP_Y_POS);
  }

  if (digitalRead(10) == LOW) {
    long randLong = random(MAX_BEES);
    Bee* b =_bee[randLong];
    b->recover();
    b->show();
    b->setLocation(random(0, 64), random(0, 100));
  }


  audio->play();

  _lastPress = shootbtn;

  return _lostGame;
}