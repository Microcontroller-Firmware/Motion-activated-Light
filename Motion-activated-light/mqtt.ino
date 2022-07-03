void mqttReconnect() {
  String clientId = "ESP8266Client-";
  clientId += String(random(0xffff), HEX);
  while (!mqttClient.connected()) {
    yield(); // Avoid soft timer
    if (mqttClient.connect(clientId.c_str(), AIO_USERNAME, AIO_KEY)) {
      String topic = mqttGetTopicName("/#");
      mqttClient.subscribe(topic.c_str());
    }
  }
}

void mqttOnMessage(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, mqttGetTopicName("/rgb").c_str()) == 0) {
    // New RGB values
    String pload = "";
    for (int i = 0; i < length; i++) {
      pload.concat((char)payload[i]);
    }
    Serial.print("New RGB values received: ");
    int red = mqttParseRgb(pload, ',', 0).toInt();
    int green = mqttParseRgb(pload, ',', 1).toInt();
    int blue = mqttParseRgb(pload, ',', 2).toInt();
    Serial.print(red);
    Serial.print(",");
    Serial.print(green);
    Serial.print(",");
    Serial.println(blue);
    
    light.setState(red, green, blue);

    mqttClient.publish(mqttGetTopicName("/rgb/state").c_str(), pload.c_str());
  } else if (strcmp(topic, mqttGetTopicName("/command").c_str()) == 0) {
    String pload = "";
    for (int i = 0; i < length; i++) {
      pload.concat((char)payload[i]);
    }
    Serial.print("New command received: ");
    Serial.println(pload);
    
    if (pload == "ON") {
      light.turnOn();
      mqttClient.publish(mqttGetTopicName("/command/state").c_str(), "ON");
      motionDetectionEnabled = false;
      mqttClient.publish(mqttGetTopicName("/motion_detection/state").c_str(), "OFF");
    } else if (pload == "OFF") {
      light.turnOff();
      mqttClient.publish(mqttGetTopicName("/command/state").c_str(), "OFF");
      motionDetectionEnabled = true;
      mqttClient.publish(mqttGetTopicName("/motion_detection/state").c_str(), "ON");
    }
  } else if (strcmp(topic, mqttGetTopicName("/brightness").c_str()) == 0) {
    String pload = "";
    for (int i = 0; i < length; i++) {
      pload.concat((char)payload[i]);
    }
    int brightness = pload.toInt();
    light.setBrightness(brightness);
    mqttClient.publish(mqttGetTopicName("/brightness/state").c_str(), pload.c_str());
  } else if (strcmp(topic, mqttGetTopicName("/motion_detection").c_str()) == 0) {
    String pload = "";
    for (int i = 0; i < length; i++) {
      pload.concat((char)payload[i]);
    }
    if (pload == "ON") {
      motionDetectionEnabled = true;
      mqttClient.publish(mqttGetTopicName("/motion_detection/state").c_str(), "ON");
    } else if (pload == "OFF") {
      motionDetectionEnabled = false;
      mqttClient.publish(mqttGetTopicName("/motion_detection/state").c_str(), "OFF");
    }
  }

}

void mqttCheckConnection() {
  if (!mqttClient.connected()) {
    Serial.println("Reconnecting mqtt");
    mqttReconnect();
  }
}

//HELPER FUNCTION
String mqttParseRgb(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String mqttGetTopicName(String topic) {
  char buf[topic.length()];
  String topicComplete = String("/") + DEVICE_ID + topic;
  return topicComplete;
}
