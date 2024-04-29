#ifndef AUDIO_H_
#define AUDIO_H_

#include <Arduino.h>

const int NOTE_LEN = 50;
const int BLASTER_LEN = 2;
const int BLASTER[BLASTER_LEN] = {415, 261};
const int ENEMY_DIVE_LEN = 5;
const int ENEMY_DIVE[ENEMY_DIVE_LEN] = {440, 440, 440, 392, 293};
// const int BLASTER[2] = {NOTE_Gs, NOTE_C};

class Audio {
  private:
    int _pin;
    int _currentTuneLen;
    const int* _currentTune;
    int _currentToneIndex;

  public:
    Audio(int pin) {
        _pin = pin;
        _currentTuneLen = -1;
        _currentTune = nullptr;
        _currentToneIndex = -1;
    }

    void play() {
      if (_currentTune == nullptr) {
        return;
      }
      if (_currentToneIndex == _currentTuneLen) {
        noTone(_pin);
        _currentTune = nullptr;
        _currentTuneLen = -1;
        _currentToneIndex = -1;
        return;
      }

      tone(_pin, _currentTune[_currentToneIndex], NOTE_LEN);
      _currentToneIndex++;
    }

    void setBlasterShot() {
      if (_currentTune != nullptr) {
        return;
      }

      _currentToneIndex = 0;
      _currentTune = BLASTER;
      _currentTuneLen = BLASTER_LEN;
    }

    void setEnemyDive() {
      if (_currentTune != nullptr) {
        return;
      }

      _currentToneIndex = 0;
      _currentTune = ENEMY_DIVE;
      _currentTuneLen = ENEMY_DIVE_LEN;
    }

};

#endif