#ifndef CLASSES
#define CLASSES
#include <Shape.hpp>

const int OFFSCREEN_COORDS = 128;

class Actor : public Rectangle {
  protected:
    int _centerw; // center of width
    const unsigned char* _sprite; // sprite byte array
    bool _drawSprite;
    const unsigned char* _spriteExplode; // sprite byte array
    bool _hidden; // actor is DED
    bool _drawCenterW; // draw the sprite from the center width of its BB (default top left)

  public:
    Actor(int x, int y, int width, int height) : Rectangle(x, y , width, height)
    {
      _centerw = width / 2;
      // _centerw = width % 2 == 1 ? width / 2 : width / 2 + 1;
      _drawSprite = true;
      _drawBoundingBox = false;
      _hidden = false;
      _drawCenterW = false;
    }

    void setSprite(const unsigned char* byteArray) {
      _sprite = byteArray;
    }

    void setDrawSprite(bool drawSprite) {
      _drawSprite = drawSprite;
    }

    void setDrawCenterW(bool drawCenterw) {
      _drawCenterW = drawCenterw;
    }

    void draw( Adafruit_SSD1306& display) {
      int centerWidthOffset = _drawCenterW ? _centerw : 0;

      if (_drawBoundingBox) {
        display.drawRect(_x - centerWidthOffset, _y, _width, _height, SSD1306_WHITE);
      }
      if (!_hidden && _drawSprite) {
        display.drawBitmap(_x - centerWidthOffset, _y, _sprite, _width, _height, SSD1306_WHITE);
      }
    }

    bool isHidden() {
      return _hidden; 
    }

    void hide() {
      _hidden = true;
      setLocation(-OFFSCREEN_COORDS, -OFFSCREEN_COORDS);
    }

    void show() {
      _hidden = false;
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

class Explosion : public Actor {
  public:
    Explosion(int x, int y, int width, int height) : Actor(x, y , width, height)
    {
    }

    String getName() const override{
      return "Explosion";
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
  private:
    int _startPosx;
    int _startPosy;
    bool _justHit; // if the enemy was just destroyed

  public:
    Enemy(int x, int y, int width, int height) : Actor(x, y , width, height)
    {
      _startPosx = x;
      _startPosy = y;
      _justHit = false;
    }

    String getName() const override{
      return "Enemy";
    }

    int getStartPositionX() {
      return _startPosx;
    }

    int getStartPositionY() {
      return _startPosy;
    }

    bool wasJustHit() {
      return _justHit;
    }

    /**
     * This enemy was hit.
    */
    void hit() {
      _justHit = true;
    }

    /**
     * Enemy is not dead anymore
    */
    void recover() {
      _justHit = false;
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