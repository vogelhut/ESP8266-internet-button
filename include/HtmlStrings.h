#pragma once

#include <Arduino.h>

const char HTML_PAGE[] PROGMEM = "<form method=\"POST\"><input type=\"text\" name=\"ssid\" placeholder=\"WiFi SSID\"></br><input type=\"password\" name=\"pass\" placeholder=\"WiFi Password\"></br><input type=\"text\" name=\"url\" placeholder=\"URL\"></br><input type=\"submit\"></form>";
const char HTML_FAIL[] PROGMEM = "<h1>One or more values rejected, press back to try again.</h1>";
const char HTML_SUCCESS[] PROGMEM = "<h1>Credentials saved! If ESP8266 does not connect, press back and try again.</h1>";