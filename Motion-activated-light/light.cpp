#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include "light.h"

Ws2812b::Ws2812b(int ledCount, int pin) {
  _light = Adafruit_NeoPixel(ledCount, pin, NEO_GRB + NEO_KHZ800);
  _light.begin();
  _pin = pin;
  _ledCount = ledCount;
  _currentState[0] = 1;
  _currentState[1] = 1;
  _currentState[2] = 1;
  _nextState[0] = 1;
  _nextState[1] = 1;
  _nextState[2] = 1;
  _savedState[0] = 80;
  _savedState[1] = 80;
  _savedState[2] = 80;
  _brightness = 255;
}

void Ws2812b::setState(int r, int g, int b) {
  _nextState[0] = r;
  _nextState[1] = g;
  _nextState[2] = b;

  _savedState[0] = r;
  _savedState[1] = g;
  _savedState[2] = b;

  Serial.print("Updated light nextState to:");
  Serial.print(_nextState[0]);
  Serial.print(",");
  Serial.print(_nextState[1]);
  Serial.print(",");
  Serial.println(_nextState[2]);
}

void Ws2812b::updateState() {
  if (!_isOn) {
    _nextState[0] = 0;
    _nextState[1] = 0;
    _nextState[2] = 0;
  }
  
  int rDiff = _currentState[0] - (((_nextState[0] * _brightness)/255) );
  int gDiff = _currentState[1] - (((_nextState[1] * _brightness)/255) );
  int bDiff = _currentState[2] - (((_nextState[2] * _brightness)/255) );

  if (rDiff == 0 && gDiff == 0 && bDiff == 0) {
    return;
  }

  _currentState[0] -= speedMap(rDiff);
  _currentState[1] -= speedMap(gDiff);
  _currentState[2] -= speedMap(bDiff);

  if (_currentState[0] < 0) {
    _currentState[0] = 0;
  } else if (_currentState[0] > 255) {
    _currentState[0] = 255;
  }
  if (_currentState[1] < 0) {
    _currentState[1] = 0;
  } else if (_currentState[1] > 255) {
    _currentState[1] = 255;
  }
  if (_currentState[2] < 0) {
    _currentState[2] = 0;
  } else if (_currentState[2] > 255) {
    _currentState[2] = 255;
  }

  setLight();
}

int Ws2812b::speedMap(int value) {
  if (value <= 30 && value >= -30)
  {
    return value;
  }

  int sign = 1;

  if (value < 0) {
    sign = -1;
  }

  return 30 * sign;
}

void Ws2812b::setLight() {
  Serial.print("Setting light: ");
  Serial.print(_currentState[0]);
  Serial.print(",");
  Serial.print(_currentState[1]);
  Serial.print(",");
  Serial.println(_currentState[2]);
  uint32_t color = _light.Color(_currentState[0],_currentState[1],_currentState[2]);
  for (int i = 0; i < _ledCount; i++) {
    _light.setPixelColor(i, color);
    //delay(2);
  }
  _light.show();
}

bool Ws2812b::turnOn() {
  _nextState[0] = _savedState[0];
  _nextState[1] = _savedState[1];
  _nextState[2] = _savedState[2];
  bool wasOn = _isOn;
  _isOn = true;
  return !(wasOn == _isOn);
}

bool Ws2812b::turnOff() {
  _nextState[0] = 0;
  _nextState[1] = 0;
  _nextState[2] = 0;
  bool wasOn = _isOn;
  _isOn = false;
  return !(wasOn == _isOn);
}

bool Ws2812b::isOn() {
  return _isOn;
}

int* Ws2812b::getState() {
  return _currentState;
}

void Ws2812b::setBrightness(int brightness) {
  _brightness = brightness;
}
