#include "graphics.h"

const int FORMATION_X_START = 2;
const int FORMATION_X_END = 57;
const int FORMATION_Y_START = 35;
const int FORMATION_Y_END = 53;

void checkCollisions(Laser** lasers, int maxLasers,
                     Bee** bees, int maxBees,
                     Ship* ship) {
  for (int i = 0; i < maxLasers; i++) {
    Laser *currentLaser = lasers[i];
    // TODO loop through galagas too
    for (int j = 0; j < maxBees; j++) {
      Bee *currentBee = bees[j];

      // check laser collisions with bee
      bool laserHit = !currentBee->isHidden() && currentLaser->overlaps(*currentBee);
      if (laserHit) {
        // TODO this breaks contract
        currentBee->hide();
        currentBee->hit();
        currentLaser->hide();
      }

      // check ship collision with bee
      bool shipHit = !currentBee->isHidden() && currentBee->overlaps(*ship);
      if (shipHit) {
        // lose life
        ship->hit();
        currentBee->hit();
      }
    }
  }
}

/**
 * Updates the score on screen.
*/
void drawScore(const String scorePrefix, int points) {
  _display.setCursor(0, 5);
  _display.print(scorePrefix);
  _display.print(points);
}

void initializeFormation(Bee** bees, int totalBees) {
  int beex = FORMATION_X_START;
  int beey = FORMATION_Y_START;

  for (int i = 0; i < totalBees; i++) {
    Bee *currentBee = bees[i];
    currentBee->setLocation(beex, beey);
    currentBee->setSprite(epd_bitmap_bee);
    currentBee->setDrawBoundingBox(false);
    currentBee->show();
    currentBee->recover();

    if (i != 0 && i % 8 == 0) {
      beex = FORMATION_X_START;
      beey = beey + 6;
    } else {
      // formationPos++;
      beex = beex + 8;
    }
  }
}

void initializeScatter(Bee** bees, int totalBees) {
  int i = totalBees;
  while ( i < totalBees) {
    long randLong = random(totalBees);
    Bee* bee = bees[randLong];
    bee->recover();
    bee->show();
    bee->setLocation(random(0, 64), random(0, 80));
    // give random initial t value
    for (int j = 0; j < randLong; j++) {
      bee->incrementT();
    }
  }
}

/**
 * totalAmount = total amount of bees
 * beewave = value from 0 - 360
*/
void drawEnemiesFormation(Bee** bees, int totalAmount, int beeWave) {
  // beeWave = (beeWave + 10) % 360;
  float radian = (beeWave * M_PI) / 180;
  for (int j = 0; j < totalAmount; j++) {
    Bee *currentBee = bees[j];
    if (currentBee->isDiving()) {
      // fly towards ship
      float t = currentBee->getT();
      // Serial.print("t: ");
      // Serial.println(t);
      // Serial.print("new x: ");
      // Serial.println((1.0 - t) * currentBee->getX() + t * currentBee->getDiveTargetX() * 1.5);
      // Serial.print("new y: ");
      // Serial.println((1.0 - t) * currentBee->getY() + t * currentBee->getDiveTargetY() * 1.5);
      int newx = ceil((1.0 - t) * currentBee->getX() + t * currentBee->getDiveTargetX() * 1.5);
      int newy = ceil((1.0 - t) * currentBee->getY() + t * currentBee->getDiveTargetY() * 1.5);
      currentBee->incrementT();

      currentBee->setLocation(newx, newy);
      // currentBee->setY(currentBee->getY() + 2);

      // if hit bottom, go back to top of screen 
      if (currentBee->getY() > _display.height()) {
        currentBee->setLocation(currentBee->getStartPositionX(), 0);
      }

      // if approaching initial position in formation, stop dive and return to formation
      // if (currentBee->getY() > _display.height()) {
      if (currentBee->getY() > currentBee->getStartPositionY() - 20 && 
          currentBee->getY() < currentBee->getStartPositionY()) {
        currentBee->setLocation(currentBee->getStartPositionX(), currentBee->getStartPositionY());
        currentBee->stopDive();
      }
    } else {
      // wave
      currentBee->setX(currentBee->getStartPositionX() + (sin(radian) * 1.1));
    }
    currentBee->draw(_display);
  }
}

void drawEnemiesScatter(Bee** bees, int totalAmount, int beeWave) {
  for (int j = 0; j < totalAmount; j++) {
    Bee *currentBee = bees[j];

    // test
    // currentBee->setLocation(currentBee->getX() + 1, currentBee->getY() + 1);
    // currentBee->setX((currentBee->getX() + 1) % _display.width());

    int randumb = (random(0, 2) - 2) * -1;
    currentBee->setX(currentBee->getX() + randumb);
    randumb = (random(0, 2) - 2) * -1;
    currentBee->setY(currentBee->getY() + randumb);
    
    // currentBee->setLocation(currentBee->getX() + randumb, currentBee->getY() + randumb);
    currentBee->setLocation(currentBee->getX() % _display.width(), currentBee->getY() % _display.height());


    currentBee->draw(_display);
  }
}


/**
 * Calculates the drawing of all lasers,
 * checking positions and redrawing.
*/
void drawLasers(Laser** lasers, int maxLasers) {
  // calculate new positions, redraw laser
  for (int i = 0; i < maxLasers; i++) {
    Laser *currentLaser = lasers[i];

    // check if current laser is now above the screen
    if (currentLaser->getY() < 0) {
      // mark it as removed
      currentLaser->hide();
    }

    if (currentLaser->isHidden()) {
      continue;
    }
    int newY = currentLaser->getY() - LASER_SPEED;
    currentLaser->setY(newY);
    currentLaser->draw(_display);
  }
}
