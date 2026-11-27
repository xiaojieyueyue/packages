#pragma once
#include <Arduino.h>

namespace EmakefunRFNano {
bool begin(uint8_t channel, uint8_t paLevel, uint8_t dataRate);
void setTxAddress(const String& addr);
void setRxAddress(const String& addr);
bool sendText(const String& text);
bool sendNumber(double value);
bool available();
String readText();
double readNumber();
}
