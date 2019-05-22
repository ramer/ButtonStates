
#include "Button.h"
#include <ESP8266WiFi.h>

Button btn(0, true);

void setup() {
  Serial.begin(74880);
  btn.onButtonDown(btn_buttonDown);
  btn.onButtonPressed(btn_buttonPressed);
  btn.onButtonUp(btn_buttonUp);
  btn.begin();
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