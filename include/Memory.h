/**
 * Lists the EEPROM offsets of permanent values
 * Anything saved in EEPROM will persist between device restarts
 */
#pragma once

// define maximum lengths of saved values, includes null terminating character
#define MAX_SSID  32
#define MAX_PASS  64
#define MAX_URL   256 // shorter than true maximum due to limited memory

// locations in EEPROM
#define EEPROM_WIFI_SSID  0
#define EEPROM_WIFI_PASS  EEPROM_WIFI_SSID + MAX_SSID
#define EEPROM_URL        EEPROM_WIFI_PASS + MAX_PASS
