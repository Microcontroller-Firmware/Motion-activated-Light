#ifndef light_h
#define light_h

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class Ws2812b

{

  public:
    Ws2812b(int ledCount, int pin);
    void setState(int r, int g, int b); // Set a new color
    void updateState(); // Call this in a regular interval to update the colors
    bool turnOn();
    bool turnOff();
    bool isOn();
    void setBrightness(int brightness);
    int* getState();

  private:
    int _ledCount;
    int _pin;
    Adafruit_NeoPixel _light;
    int _currentState [3];
    int _nextState [3];
    int _savedState [3];
    int _brightness;
    bool _isOn;

    void setLight();
    int speedMap(int value);
};

#endif
