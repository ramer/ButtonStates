// Button.cpp

#include "Button.h"

Button::Button(byte pin_number, bool inverted = false) {
  pin = pin_number;
  pin_inverted = inverted;
}

void Button::begin() {
  pinMode(pin, INPUT);
  pin_average = digitalRead(pin) ^ pin_inverted;  // initial value

  running = true;
}

void Button::stop() {
  running = false;
}

ButtonState Button::update()
{
  if (!running) return STOPPED;
  if (millis() - update_time < INTERVAL_UPDATE) return pin_laststate;

  pin_average = (pin_average * (PIN_AVERAGE - 1) + (digitalRead(pin) ^ pin_inverted)) / PIN_AVERAGE;

  if (pin_average > 0.8 && pin_laststate == RELEASED)
  {
    pin_time = millis();
    pin_laststate = DOWN;
    buttonDownCallback();
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
