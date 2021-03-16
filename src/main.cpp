#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define BUTTON D1
#define LED D4

#define WIFI_SSID ""
#define WIFI_PASS ""
char server[] = "www.google.com";
WiFiClientSecure client;

int buttonState = LOW;

void setup()
{
  pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT);

  // Serial setup
  Serial.begin(115200);
  while (!Serial); // Wait for Serial to connect

  // WiFi setup
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected");

  client.setInsecure();
}

void loop()
{
  int buttonRead = digitalRead(BUTTON);

  if (buttonRead == HIGH)
  {
    digitalWrite(LED, LOW);
  } else {
    digitalWrite(LED, HIGH);
  }

  if (buttonRead != buttonState)
  {
    buttonState = buttonRead;

    // button pressed
    if (buttonState == LOW)
    {
      Serial.println("Button has been pressed, making request");
      HTTPClient request;
      if (request.begin(client, server))
      {
        // Make HTTP request, we don't care about the response
        request.GET();
        // debounce
        delay(5000);
      }
    }
  }
}
