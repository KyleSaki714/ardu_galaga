#ifndef ACTORS_H_
#define ACTORS_H_

#include "Arduino.h"
#include <Shape.hpp>
#include <Fonts/Picopixel.h>

const int OFFSCREEN_COORDS = 128;
const float T_INTERPOLATION_SPEED = 0.001;

class Actor : public Rectangle {
  protected:
    int _centerw; // center of width
    const unsigned char* _sprite; // sprite byte array
    bool _drawSprite;
    const unsigned char* _spriteExplode; // sprite byte array
    bool _hidden; // actor is DED
    bool _drawCenterW; // draw the sprite from the center width of its BB (default top left)
    bool _justHit; // if the Actor was just destroyed (collided with something else)

  public:
    Actor(int x, int y, int width, int height);
    void setSprite(const unsigned char* byteArray);
    void setDrawSprite(bool drawSprite);
    void setDrawCenterW(bool drawCenterw);
    void draw( Adafruit_SSD1306& display);
    bool isHidden();
    void hide();
    void show();
    bool wasJustHit();
    void hit();
    void recover();
    String getName() const override;
};

class Laser : public Actor {
  public:
    Laser(int x, int y, int width, int height);
    String getName() const override;
};

class Explosion : public Actor {
  public:
    Explosion(int x, int y, int width, int height);
    String getName() const override;
};

class Ship : public Actor {
  public:
    Ship(int x, int y, int width, int height);
    String getName() const override;
};

class Enemy : public Actor {
  private:
    int _startPosx;
    int _startPosy;
    bool _isDiving;
    float _t; // interpolation factor
    int _diveTargetx;
    int _diveTargety;

  public:
    Enemy(int x, int y, int width, int height);
    int getStartPositionX();
    int getStartPositionY();
    bool isDiving();
    void startDive(int targetx, int targety);
    void stopDive();
    int getDiveTargetX();
    int getDiveTargetY();
    float getT();
    void incrementT();
    String getName() const override;
};

class Bee : public Enemy {
  public:
    Bee(int x, int y, int width, int height);
    String getName() const override;
};

class BossGalaga : public Enemy {
  public:
    BossGalaga(int x, int y, int width, int height);
    String getName() const override;
};

#endif