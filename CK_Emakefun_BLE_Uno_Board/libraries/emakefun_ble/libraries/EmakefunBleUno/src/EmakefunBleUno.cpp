#include "EmakefunBleUno.h"

namespace EmakefunBleUno {
namespace {
bool started = false;
void ensureStarted() {
  if (!started) {
    begin(115200);
  }
}
}

void begin(unsigned long baud) {
  Serial.begin(baud);
  started = true;
  delay(50);
}

void sendAt(const String& cmd) {
  ensureStarted();
  Serial.println(cmd);
  delay(60);
}

void setName(const String& name) {
  sendAt(String("AT+NAME=") + name);
}

void setRole(int role) {
  sendAt(String("AT+ROLE=") + String(role ? 1 : 0));
}

void setRoute(int mode) {
  if (mode < 0 || mode > 2) mode = 0;
  sendAt(String("AT+BLEUSB=") + String(mode));
}

void connectMac(const String& mac) {
  sendAt(String("AT+CON=") + mac);
}

void sendText(const String& text, bool newline) {
  ensureStarted();
  if (newline) Serial.println(text);
  else Serial.print(text);
}

bool available() {
  ensureStarted();
  return Serial.available() > 0;
}

String readText() {
  ensureStarted();
  String out;
  while (Serial.available() > 0) {
    out += static_cast<char>(Serial.read());
    delay(2);
  }
  out.trim();
  return out;
}
}
