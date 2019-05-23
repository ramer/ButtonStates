// Button.cpp

#include "Button.h"

Button::Button(byte pin_number) {
  pin = pin_number;
}

void Button::begin(bool inverted = false) {
  pinMode(pin, INPUT);

  pin_inverted = inverted;
  pin_average = digitalRead(pin) ^ pin_inverted;  // initial value

  running = true;
}

void Button::stop() {
  running = false;
}

ButtonState Button::update()
{
  if (!running) return STOPPED;
  if (!(millis() - update_time > INTERVAL_UPDATE)) return pin_laststate;
  
  update_time = millis();
  
  pin_average = (pin_average * (PIN_AVERAGE - 1) + (digitalRead(pin) ^ pin_inverted)) / PIN_AVERAGE;

  if (pin_average > 0.8 && pin_laststate == RELEASED)
  {
    pin_time = millis();
    pin_laststate = DOWN;
    buttonDownCallback();
    
    if (pin_payload != 0) {
      *pin_payload = !*pin_payload;
      payloadUpdatedCallback(pin_payload);
    }

    return pin_laststate;
  }

  if (pin_average > 0.8 && pin_laststate == DOWN)
  {
    pin_laststate = PRESSED;
    buttonPressedCallback();
    return pin_laststate;
  }

  if (pin_average < 0.2 && (pin_laststate == DOWN || pin_laststate == PRESSED))
  {
    pin_laststate = UP;
    buttonUpCallback(millis() - pin_time);
    return pin_laststate;
  }

  if (pin_average < 0.2 && pin_laststate == UP)
  {
    pin_laststate = RELEASED;
    return pin_laststate;
  }

  return pin_laststate;
}

ulong Button::pressedTime()
{
  return pin_laststate == PRESSED ? millis() - pin_time : 0;
}

void Button::setPayload(bool *payload)
{
  pin_payload = payload;
}

void Button::removePayload()
{
  pin_payload = 0;
}

void Button::onButtonDown(std::function<void()> onButtonDownCallback) {
  buttonDownCallback = onButtonDownCallback;
}

void Button::onButtonPressed(std::function<void()> onButtonPressedCallback)
{
  buttonPressedCallback = onButtonPressedCallback;
}

void Button::onButtonUp(std::function<void(ulong)> onButtonUpCallback)
{
  buttonUpCallback = onButtonUpCallback;
}

void Button::onPayloadUpdated(std::function<void(bool*)> onPayloadUpdatedCallback)
{
  payloadUpdatedCallback = onPayloadUpdatedCallback;
}
