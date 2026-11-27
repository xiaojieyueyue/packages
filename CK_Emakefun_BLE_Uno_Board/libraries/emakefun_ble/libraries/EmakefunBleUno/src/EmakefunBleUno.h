#pragma once
#include <Arduino.h>

namespace EmakefunBleUno {
void begin(unsigned long baud);
void sendAt(const String& cmd);
void setName(const String& name);
void setRole(int role);
void setRoute(int mode);
void connectMac(const String& mac);
void sendText(const String& text, bool newline);
bool available();
String readText();
}
