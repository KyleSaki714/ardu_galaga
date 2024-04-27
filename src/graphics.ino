#ifndef BRUH_G
#define BRUH_G
#include <Shape.hpp>

class Actor : public Rectangle {
  protected:
    int _centerw; // center of width
    const unsigned char* _sprite; // sprite byte array
    bool _drawSprite;
    const unsigned char* _spriteExplode; // sprite byte array
    bool _exploded; // actor is DED

  public:
    Actor(int x, int y, int width, int height) : Rectangle(x, y , width, height)
    {
      _centerw = width / 2;
      // _centerw = width % 2 == 1 ? width / 2 : width / 2 + 1;
      _drawSprite = true;
      _drawBoundingBox = false;
      _exploded = false;
    }

    void setSprite(const unsigned char* byteArray) {
      _sprite = byteArray;
    }

    void setDrawSprite(bool drawSprite) {
      _drawSprite = drawSprite;
    }

    void draw( Adafruit_SSD1306& display) {
      if (_drawBoundingBox) {
        display.drawRect(_x - _centerw, _y, _width, _height, SSD1306_WHITE);
      }
      if (_exploded) {
        display.drawBitmap(_x - _centerw, _y, _spriteExplode, _width, _height, SSD1306_WHITE);
      } else if (_drawSprite) {
        display.drawBitmap(_x - _centerw, _y, _sprite, _width, _height, SSD1306_WHITE);
      }
    }

    void setExplodeSprites(const unsigned char* byteArray) {
      _spriteExplode = byteArray;
    }

    // explode 4, 5, 6
    void explode() {
      _exploded = true;
    }

    String getName() const override{
      return "Actor";
    }
};

class Laser : public Actor {
  public:
    Laser(int x, int y, int width, int height) : Actor(x, y , width, height)
    {
    }

    String getName() const override{
      return "Laser";
    }
};

class Ship : public Actor {
  public:
    Ship(int x, int y, int width, int height) : Actor(x, y , width, height)
    {
    }

    String getName() const override{
      return "Ship";
    }
};

class Enemy : public Actor {
  public:
    Enemy(int x, int y, int width, int height) : Actor(x, y , width, height)
    {
    }

    String getName() const override{
      return "Enemy";
    }
};

class Bee : public Enemy {
  public:
    Bee(int x, int y, int width, int height) : Enemy(x, y , width, height)
    {
    }

    String getName() const override{
      return "Bee";
    }
};

class BossGalaga : public Enemy {
  public:
    BossGalaga(int x, int y, int width, int height) : Enemy(x, y , width, height)
    {
    }

    String getName() const override{
      return "BossGalaga";
    }
};

#endif