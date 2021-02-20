#include <Arduino.h>
#include <ESP8266WiFi.h>

#define BUTTON D4
#define LED D0

#define WIFI_SSID ""
#define WIFI_PASS ""
char server[] = "www.google.com";
WiFiClient client;

void setup() 
{
  pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT);

  // WiFi setup
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
}

void loop() 
{
  if (digitalRead(BUTTON) == HIGH)
  {
    if (client.connect(server, 80)) 
    {
      // Make HTTP request:
      client.println("GET /search?q=arduino HTTP/1.0");
      client.println();
      // debounce
      delay(2000);
    }
  }
}
