#include "Arduino.h"
#include "analog_sensor.h"

AnalogSensor::AnalogSensor(int pin, unsigned long pollMilliseconds, unsigned long cooldownMilliseconds, int minChange) {
  _pin = pin;
  _pollMilliseconds = pollMilliseconds;
  _cooldownMilliseconds = cooldownMilliseconds;
  _lastReading = 0;
  _stateChanged = true;
  _state = analogRead(pin);
  _minChange = minChange;
}

void AnalogSensor::read() {
  // Handle overflows
  if (millis() < _lastReading) {
    _lastReading = 0;
  }

  unsigned long msGone = millis() - _lastReading;
  if (msGone > _pollMilliseconds) {
    int newState = analogRead(_pin);
    if (_state != newState && msGone < _cooldownMilliseconds) {
      return;
    }
    if (abs(_state - newState) < _minChange) {
      return;
    }
    if (newState != _state) {
      _stateChanged = true;
    }
    _state = newState;
    _lastReading = millis();
  }
}

int AnalogSensor::getState() {
  _stateChanged = false;
  return _state;
}

float AnalogSensor::getStatePercent() {
  _stateChanged = false;
  return _state / 1024.0;
}

bool AnalogSensor::hasStateChanged() {
  return _stateChanged;
}
