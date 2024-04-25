#ifndef BRUH_G
#define BRUH_G
#include <Shape.hpp>

// TODO: class Actor : public Rectangle

class Laser : public Rectangle {
  public:

    Laser(int x, int y, int width, int height) : Rectangle(x, y , width, height)
    {
    }

    String getName() const override{
      return "Laser";
    }
};

class Ship : public Rectangle {
  protected:
    int _centerw; // center of width

  public:
    Ship(int x, int y, int width, int height) : Rectangle(x, y , width, height)
    {
      // _centerw = width % 2 == 1 ? width / 2 : width / 2 + 1;
      _centerw = width / 2;
    }

    void draw( Adafruit_SSD1306& display) {
      if (_drawBoundingBox) {
        display.drawRect(_x - _centerw, _y, _width, _height, SSD1306_WHITE);
      }
    }

    String getName() const override{
      return "Ship";
    }
};

#endif