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
  
  // avg      = (             avg * 3             +             0 or 255                    + 2) /  4
  pin_average = ((pin_average << 2) - pin_average + (digitalRead(pin) ^ pin_inverted) * 255 + 2) >> 2; // for uint16
  // pin_average = (pin_average * (PIN_AVERAGE - 1) + (digitalRead(pin) ^ pin_inverted)) / PIN_AVERAGE; // for float

  if (pin_laststate == RELEASED && pin_average > 223)
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

  if (pin_laststate == DOWN && pin_average > 223)
  {
    pin_laststate = PRESSED;
    buttonPressedCallback();
    return pin_laststate;
  }

  if ((pin_laststate == DOWN || pin_laststate == PRESSED) && pin_average < 32)
  {
    pin_laststate = UP;
    buttonUpCallback(millis() - pin_time);
    return pin_laststate;
  }

  if (pin_laststate == UP && pin_average < 32)
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
