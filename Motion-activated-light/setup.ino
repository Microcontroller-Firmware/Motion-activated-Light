void setupWifi() {
  delay(2);
  WiFi.begin(WLAN_SSID, WLAN_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  randomSeed(micros());
}

void setupMqtt () {
  mqttClient.setServer(AIO_SERVER, AIO_SERVERPORT);
  mqttClient.setCallback(mqttOnMessage);
}
