#include "Arduino.h"
#include "digital_sensor.h"

DigitalSensor::DigitalSensor(int pin, unsigned long pollMilliseconds, unsigned long cooldownRisingMilliseconds, unsigned long cooldownFallingMilliseconds) {
  _pin = pin;
  _pollMilliseconds = pollMilliseconds;
  _cooldownRisingMilliseconds = cooldownRisingMilliseconds;
  _cooldownFallingMilliseconds = cooldownFallingMilliseconds;
  _lastReading = 0;
  _stateChanged = true;
  _state = digitalRead(pin);
}

void DigitalSensor::read() {
  // Handle overflows
  if (millis() < _lastReading) {
    _lastReading = 0;
  }

  unsigned long msGone = millis() - _lastReading;
  if (msGone > _pollMilliseconds) {
    if (_state == HIGH && msGone < _cooldownFallingMilliseconds) {
      return;
    }
    if (_state == LOW && msGone < _cooldownRisingMilliseconds) {
      return;
    }

    int newState = digitalRead(_pin);
    if (newState != _state) {
      _stateChanged = true;
    }
    _state = newState;
    _lastReading = millis();
  }
}

int DigitalSensor::getState() {
  _stateChanged = false;
  return _state;
}

bool DigitalSensor::hasStateChanged() {
  return _stateChanged;
}
