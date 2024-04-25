#ifndef BRUH_G
#define BRUH_G
#include <Shape.hpp>

class Actor : public Rectangle {
  protected:
    int _centerw; // center of width
    const unsigned char* _sprite; // sprite byte array
    bool _drawSprite;

  public:
    Actor(int x, int y, int width, int height) : Rectangle(x, y , width, height)
    {
      _centerw = width / 2;
      // _centerw = width % 2 == 1 ? width / 2 : width / 2 + 1;
      _drawSprite = true;
      _drawBoundingBox = false;
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
      if (_drawSprite) {
        display.drawBitmap(_x - _centerw, _y, _sprite, _width, _height, SSD1306_WHITE);
      }
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

#endif