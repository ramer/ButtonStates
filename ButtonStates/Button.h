// Button.h

#ifndef _BUTTON_h
#define _BUTTON_h

#include <ESP8266WiFi.h>

#define INTERVAL_UPDATE	              5

enum ButtonState { STOPPED = -1, RELEASED = 0, DOWN = 1, PRESSED = 2, UP = 4 };

class Button
{
public:
  Button(byte pin_number);
  
  void begin(bool inverted);
  void stop();
  ButtonState update();
  ulong pressedTime();
  void setPayload(bool *payload);
  void removePayload();

  void onButtonDown(std::function<void()> onButtonDownCallback);
  void onButtonPressed(std::function<void()> onButtonPressedCallback);
  void onButtonUp(std::function<void(ulong)> onButtonUpCallback);
  void onPayloadUpdated(std::function<void(bool*)> onPayloadUpdatedCallback);

protected:
  bool running = false;
  ulong update_time;

  byte         pin;
  bool         pin_inverted = false;
  ButtonState  pin_laststate = RELEASED;
  uint16_t     pin_average;
  ulong        pin_time;
  bool        *pin_payload;

  std::function<void()> buttonDownCallback;
  std::function<void()> buttonPressedCallback;
  std::function<void(ulong)> buttonUpCallback;
  std::function<void(bool*)> payloadUpdatedCallback;
};

#endif

