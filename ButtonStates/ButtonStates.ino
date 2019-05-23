
#include "Button.h"
#include <ESP8266WiFi.h>

Button btn(13);
bool btn_payload = false;

void setup() {
  Serial.begin(74880);
  btn.onButtonDown(btn_buttonDown);
  btn.onButtonPressed(btn_buttonPressed);
  btn.onButtonUp(btn_buttonUp);
  btn.onPayloadUpdated(btn_payloadUpdated);
  btn.setPayload(&btn_payload);
  btn.begin(true);
}

void loop() {
  btn.update(); // can return current state
}

void btn_buttonDown() {
  Serial.println("Button DOWN");
}

void btn_buttonPressed() {
  Serial.println("Button PRESSED");
}

void btn_buttonUp(ulong time) {
  Serial.printf("Button UP, was PRESSED for %u ms \n", time);
}

void btn_payloadUpdated(bool *payload) {
  Serial.printf("Button payload updated, new value is %u, original variable value is %u \n", *payload, btn_payload);
}