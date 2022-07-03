# Motion activated Light

This is a Firmware to have motion activated lighting that can also be controlled manually via MQTT and is fully compatible with Home Assistant.

## Hardware

You will need:

* Wemos D1 Mini
* Generic PIR Sensor
* Generic LDR 
* WS2812B LEDs
* Resistor 
* Button (optional)

Attach the LDR to the `A0` Pin with a voltage divider circuit. Attach the PIR to the `PIRPIN` and the LEDs to the `LEDPIN`. 

If you want to use the button to turn on and off the light you need to attach the button so it connects `BUTTONPIN` to ground.

## Installation

1. Open the project in the Arduino IDE 
2. Set the settings in the `Motion-activated-light.ino` file 
3. Flash your microcontroller with the code

## Home Assistant

Assuming your `DEVICE_ID` is `001` add this light:

```
- platform: mqtt
  name: "Light 001"
  state_topic: "/001/command/state"
  command_topic: "/001/command"
  brightness_state_topic: "/001/brightness/state"
  brightness_command_topic: "/001/brightness"
  brightness_scale: 255
  rgb_state_topic: "/001/rgb/state"
  rgb_command_topic: "/001/rgb"
  qos: 0
  payload_on: "ON"
  payload_off: "OFF"
  optimistic: false
  retain: true
```

this switch:
```
- platform: mqtt
  name: Light 001 Motion Detection
  state_topic: "/001/motion_detection/state"
  command_topic: "/001/motion_detection"
  payload_on: "ON"
  payload_off: "OFF"
  state_on: "ON"
  state_off: "OFF"
  retain: true
```

and those sensors:

```
- platform: mqtt
  name: "Illumination 001"
  state_topic: "/001/light/state"
  unit_of_measurement: "%"
  value_template: "{{ value | multiply(0.09765625) }}"
- platform: mqtt
  name: "Motion 001"
  state_topic: "/001/motion/state"
  unit_of_measurement: "Bool"
```
