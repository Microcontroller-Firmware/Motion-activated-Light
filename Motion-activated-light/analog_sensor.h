#ifndef analog_sensor_h
#define analog_sensor_h

#include "Arduino.h"

class AnalogSensor

{

  public:
    AnalogSensor(int pin, unsigned long pollMilliseconds, unsigned long cooldownMilliseconds, int minChange);
    void read(); // Call this in a regular interval to update the readings
    int getState();
    float getStatePercent();
    bool hasStateChanged();

  private:
    int _state;
    unsigned long _pollMilliseconds;
    unsigned long _cooldownMilliseconds;
    unsigned long _lastReading;
    int _pin;
    int _minChange;
    bool _stateChanged;
};

#endif
