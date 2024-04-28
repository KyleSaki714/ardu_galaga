#ifndef INPUT_H_
#define INPUT_H_

#include "Arduino.h"

class Input {
  private:
    int _input;

  public:
    Input() {

    }
};

class Button {
  private:
    int _pin;
    int _lastPress;
    bool _pullUp;

  public:
    Button(int aButtonPin, bool internalPullupTrue) {
      _pin = aButtonPin;
      _lastPress = internalPullupTrue ? HIGH : LOW;
      _pullUp = internalPullupTrue;
    }

    int readButtonRaw() {
      return digitalRead(_pin);
    }

    int readButtonDebounce() {
      int val = readButtonRaw();
      return val != _lastPress &&
             val == _pullUp ? LOW : HIGH;
    }
};

#endif