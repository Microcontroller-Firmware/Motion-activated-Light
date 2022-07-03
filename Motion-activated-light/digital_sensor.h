#ifndef digital_sensor_h
#define digital_sensor_h

#include "Arduino.h"

class DigitalSensor

{

  public:
    DigitalSensor(int pin, unsigned long pollMilliseconds, unsigned long cooldownRisingMilliseconds, unsigned long cooldownFallingMilliseconds);
    void read(); // Call this in a regular interval to update the readings
    int getState();
    bool hasStateChanged();

  private:
    int _state;
    unsigned long _pollMilliseconds;
    unsigned long _cooldownRisingMilliseconds;
    unsigned long _cooldownFallingMilliseconds;
    unsigned long _lastReading;
    int _pin;
    bool _stateChanged;
};

#endif
