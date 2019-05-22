// Button.h

#ifndef _BUTTON_h
#define _BUTTON_h


#include <ESP8266WiFi.h>

#define INTERVAL_UPDATE	              10
#define INTERVAL_BUTTON_DEBOUNCE	    100
#define INTERVAL_BUTTON_IDLE	        200
#define PIN_AVERAGE                   10

enum ButtonState { STOPPED = -1, RELEASED = 0, DOWN = 1, PRESSED = 2, UP = 4 };

class Button
{
public:
  Button(byte pin_number, bool inverted);
  
  void begin();
  void stop();
  ButtonState update();
  ulong pressedTime();

  void onButtonDown(std::function<void()> onButtonDownCallback);
  void onButtonPressed(std::function<void()> onButtonPressedCallback);
  void onButtonUp(std::function<void(ulong)> onButtonUpCallback);


protected:
  std::function<void()> buttonDownCallback;
  std::function<void()> buttonPressedCallback;
  std::function<void(ulong)> buttonUpCallback;

  bool running = false;

  byte  pin;
  bool  pin_inverted = false;
  ButtonState  pin_laststate = RELEASED;
  float pin_average;
  ulong pin_time;

  ulong update_time;
};

#endif

