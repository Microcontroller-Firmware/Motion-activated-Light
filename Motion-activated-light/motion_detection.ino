void handleMotionDetection() {
  if (!motionDetectionEnabled) {
    return;
  }

  if (!light.isOn() && pirSensor.getState() == 1 && ldrSensor.getState() < CRITICALLIGHT) {
    Serial.println("Motion detection light on");
    int newRed = ldrSensor.getState()*ldrSensor.getState()*0.001 + 15;
    int newGreen = newRed /4;
    light.setState(newRed,newGreen,0);
    light.setBrightness(255);
    light.turnOn();
    char buffer[11];
    sprintf(buffer, "%d,%d,%d", newRed, newGreen, 0);
    Serial.println("1");
    mqttClient.publish(mqttGetTopicName("/rgb/state").c_str(), buffer);
    Serial.println("2");
    mqttClient.publish(mqttGetTopicName("/command/state").c_str(), "ON");
    Serial.println("3");
    mqttClient.publish(mqttGetTopicName("/brightness/state").c_str(), "255");
    Serial.println("4");
  } else if (light.isOn() && pirSensor.getState() == 0) {
    Serial.println("Motion detection light off");
    light.turnOff();
    mqttClient.publish(mqttGetTopicName("/command/state").c_str(), "OFF");
  }
}
