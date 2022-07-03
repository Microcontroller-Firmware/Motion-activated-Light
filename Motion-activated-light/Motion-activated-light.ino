#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "light.h"
#include "digital_sensor.h"
#include "analog_sensor.h"

/*
 * Configuration Area
 *
 * Set your variables here, that is all you need to touch.
 */

#define LEDPIN 13   // pin the WS2812B strip is connected to
#define LEDCOUNT 36 // amount of single LEDs // TODO increase to 36

#define PIRPIN 4                  // pin the PIR sensor is connected to
#define PIRCOOLDOWNRISING 250     // Milliseconds until rising will be processed
#define PIRCOOLDOWNFALLING 120000 // Milliseconds until falling will be processed
#define BUTTONPIN 14              // D5 connect button here to GND
#define CRITICALLIGHT 40          // Threshold to use motion detection 1-1024

// Your WiFi credentials
#define WLAN_SSID "ssid"
#define WLAN_PASS "********"

// Your mqtt credentials
#define AIO_SERVER "mqtt.example.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "mqtt_user"
#define AIO_KEY "mqtt_password"

#define DEVICE_ID "001" // Used for the mqtt channels

/*
 * Code Area
 *
 * No need to touch anything below here.
 */

bool motionDetectionEnabled = false;
bool initialConnect = true;
int stopMqttUntil = 0;
int lastInterrupt = 0;

// Constructors
Ws2812b light = Ws2812b(LEDCOUNT, LEDPIN);
DigitalSensor pirSensor = DigitalSensor(PIRPIN, 10, PIRCOOLDOWNRISING, PIRCOOLDOWNFALLING);
AnalogSensor ldrSensor = AnalogSensor(A0, 1000, 0, 10);

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setup()
{
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  delay(10);

  Serial.println("Booted, starting setup ...");

  // Connect to WiFi access point.
  Serial.print("Connecting to wifi ");
  Serial.print(WLAN_SSID);
  Serial.println(" ...");
  setupWifi();

  // Setup MQTT subscription for onoff feed.
  Serial.print("Setting up mqtt connection to ");
  Serial.print(AIO_SERVER);
  Serial.println(" ...");
  setupMqtt();

  Serial.println("Setting up button(s) ...");

  pinMode(BUTTONPIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTONPIN), buttonPressed, FALLING);

  Serial.println("Finished setup!");
}

ICACHE_RAM_ATTR void buttonPressed()
{
  if (millis() - lastInterrupt < 500)
  {
    return;
  }

  Serial.println("Button is pressed");
  lastInterrupt = millis();
  if (!light.isOn())
  {
    motionDetectionEnabled = false;
    int newRed = ldrSensor.getState() * ldrSensor.getState() * 0.001 + 15;
    int newGreen = newRed / 4;
    light.turnOn();
    light.setBrightness(255);
    light.setState(newRed, newGreen, 0);
    mqttClient.publish(mqttGetTopicName("/rgb/state").c_str(), (String(newRed) + String(",") + String(newGreen) + String(",0")).c_str());
    mqttClient.publish(mqttGetTopicName("/command/state").c_str(), "ON");
    mqttClient.publish(mqttGetTopicName("/brightness/state").c_str(), "255");
  }
  else
  {
    motionDetectionEnabled = true;
    light.turnOff();
    mqttClient.publish(mqttGetTopicName("/command/state").c_str(), "OFF");
  }
}

void publishLdr()
{
  if (ldrSensor.hasStateChanged())
  {
    mqttClient.publish(mqttGetTopicName("/light/state").c_str(), String(ldrSensor.getState()).c_str());
    Serial.print("LDR state: ");
    Serial.println(ldrSensor.getState());
  }
}

void publishPir()
{
  if (pirSensor.hasStateChanged())
  {
    mqttClient.publish(mqttGetTopicName("/motion/state").c_str(), String(pirSensor.getState()).c_str());
    Serial.print("PIR state: ");
    Serial.println(pirSensor.getState());
  }
}

// the loop routine runs over and over again forever:
void loop()
{
  mqttClient.loop();
  mqttCheckConnection();
  delay(150); // Give the mqtt lib time to sync the messages
  ldrSensor.read();
  pirSensor.read();
  publishLdr();
  publishPir();
  handleMotionDetection();

  if (millis() > 500)
  {
    // Give some time to restore the last state
    light.updateState();
  }
}
