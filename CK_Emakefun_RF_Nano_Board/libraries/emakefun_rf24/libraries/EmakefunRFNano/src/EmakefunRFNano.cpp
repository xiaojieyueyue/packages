#include "EmakefunRFNano.h"
#include <SPI.h>
#include <RF24.h>
#include <string.h>

namespace EmakefunRFNano {
namespace {
// Official Emakefun RF-Nano V3.0 onboard nRF24L01+ wiring: CE=D7, CSN=D8
RF24 radio(7, 8);
bool initialized = false;
byte txAddr[6] = "00001";
byte rxAddr[6] = "00001";

void copyAddr(const String& src, byte dst[6]) {
  memset(dst, '0', 5);
  dst[5] = 0;
  size_t len = src.length();
  if (len > 5) len = 5;
  for (size_t i = 0; i < len; ++i) {
    dst[i] = static_cast<byte>(src[i]);
  }
}

void ensureInit() {
  if (!initialized) {
    begin(115, 3, 1);
  }
}
}

bool begin(uint8_t channel, uint8_t paLevel, uint8_t dataRate) {
  if (channel > 125) channel = 125;
  bool ok = radio.begin();
  initialized = true;
  radio.setAddressWidth(5);
  radio.setChannel(channel);
  switch (paLevel) {
    case 0: radio.setPALevel(RF24_PA_MIN); break;
    case 1: radio.setPALevel(RF24_PA_LOW); break;
    case 2: radio.setPALevel(RF24_PA_HIGH); break;
    default: radio.setPALevel(RF24_PA_MAX); break;
  }
  switch (dataRate) {
    case 0: radio.setDataRate(RF24_250KBPS); break;
    case 2: radio.setDataRate(RF24_2MBPS); break;
    default: radio.setDataRate(RF24_1MBPS); break;
  }
  radio.openWritingPipe(txAddr);
  radio.openReadingPipe(1, rxAddr);
  radio.startListening();
  return ok;
}

void setTxAddress(const String& addr) {
  ensureInit();
  copyAddr(addr, txAddr);
  radio.openWritingPipe(txAddr);
  radio.stopListening();
}

void setRxAddress(const String& addr) {
  ensureInit();
  copyAddr(addr, rxAddr);
  radio.openReadingPipe(1, rxAddr);
  radio.startListening();
}

bool sendText(const String& text) {
  ensureInit();
  char buf[32];
  memset(buf, 0, sizeof(buf));
  size_t len = text.length();
  if (len > 31) len = 31;
  memcpy(buf, text.c_str(), len);
  radio.stopListening();
  return radio.write(buf, sizeof(buf));
}

bool sendNumber(double value) {
  return sendText(String(value, 4));
}

bool available() {
  ensureInit();
  return radio.available();
}

String readText() {
  ensureInit();
  if (!radio.available()) return String();
  char buf[33];
  memset(buf, 0, sizeof(buf));
  uint8_t bytes = radio.getPayloadSize();
  if (bytes > 32) bytes = 32;
  radio.read(buf, bytes);
  buf[32] = '\0';
  return String(buf);
}

double readNumber() {
  String s = readText();
  if (s.length() == 0) return 0.0;
  return s.toFloat();
}
}
