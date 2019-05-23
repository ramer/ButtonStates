
#include "Button.h"
#include <ESP8266WiFi.h>

Button btn1(13);
Button btn2(0);
bool btn_payload = false;

void setup() {
  Serial.begin(74880);
  btn1.onButtonDown(btn_buttonDown);
  btn1.onButtonPressed(btn_buttonPressed);
  btn1.onButtonUp(btn_buttonUp);
  btn1.onPayloadUpdated(btn_payloadUpdated);
  btn1.setPayload(&btn_payload);
  btn1.begin(true);

  btn2.onButtonDown(btn_buttonDown);
  btn2.onButtonPressed(btn_buttonPressed);
  btn2.onButtonUp(btn_buttonUp);
  btn2.onPayloadUpdated(btn_payloadUpdated);
  btn2.setPayload(&btn_payload);
  btn2.begin(true);
}

void loop() {
  btn1.update(); // can return current state
  btn2.update(); // can return current state
}

void btn_buttonDown(Button *sender) {
  if (sender == &btn1) {
    Serial.println("Button 1 DOWN");
  }
  else if (sender == &btn2) {
    Serial.println("Button 2 DOWN");
  }
  else {
    Serial.println("Unknown button DOWN");
  }
}

void btn_buttonPressed(Button *sender) {
  Serial.println("Button PRESSED");
}

void btn_buttonUp(Button *sender, ulong time) {
  Serial.printf("Button UP, was PRESSED for %u ms \n", time);
}

void btn_payloadUpdated(Button *sender, bool *payload) {
  Serial.printf("Button payload updated, new value is %u, original variable value is %u \n", *payload, btn_payload);
}