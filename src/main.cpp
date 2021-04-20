#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include "HtmlStrings.h"
#include "Memory.h"

#define BUTTON D1
#define LED D4

WiFiClientSecure client;
ESP8266WebServer server(80);
// for some reason WiFi.begin() doesn't like empty strings so they're initialized to random values
char ssid[MAX_SSID] = "a";
char pass[MAX_PASS] = "b";
char url[MAX_URL];
int buttonState = LOW;

void handleGet()
{
  server.send_P(200, "text/html", HTML_PAGE);
}

void handlePost()
{
  String _ssid = server.arg("ssid");
  String _pass = server.arg("pass");
  String _url = server.arg("url");
  if (_ssid == emptyString || _pass == emptyString || _url == emptyString ||
      _ssid.length() > MAX_SSID || _pass.length() > MAX_PASS || _url.length() > MAX_URL)
  {
    server.send_P(400, "text/html", HTML_FAIL); // The request is invalid, so send HTTP status 400
  }
  else
  {
    strncpy(ssid, _ssid.c_str(), MAX_SSID);
    strncpy(pass, _pass.c_str(), MAX_PASS);
    strncpy(url, _url.c_str(), MAX_URL);
    EEPROM.put(EEPROM_WIFI_SSID, ssid);
    EEPROM.put(EEPROM_WIFI_PASS, pass);
    EEPROM.put(EEPROM_URL, url);

    WiFi.begin(ssid, pass); // restart wifi with new ssid and password
    server.send_P(200, "text/html", HTML_SUCCESS);
  }
}

void setup()
{
  pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT);

  // Serial setup //
  Serial.begin(115200);
  while (!Serial); // Wait for Serial to connect
  Serial.println("Initializing...");

  // Get values from EEPROM //
  EEPROM.begin(512);

  EEPROM.get(EEPROM_WIFI_SSID, ssid);
  EEPROM.get(EEPROM_WIFI_PASS, pass);
  EEPROM.get(EEPROM_URL, url);

  // WiFi setup //
  WiFi.begin(ssid, pass);
  WiFi.softAP("ESP8266-Internet-Button");
  server.on("/", HTTP_GET, handleGet);
  server.on("/", HTTP_POST, handlePost);
  server.begin();

  Serial.print("Connecting to WiFi");
  unsigned long connStart = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED, !digitalRead(LED));
    Serial.print(".");
    delay(500);
    if (millis() - connStart > 5000)
    {
      // WiFi hasn't connected in 5 seconds, start handling softAP for incoming config
      server.handleClient(); // must visit 192.168.4.1 to get page
    }
  }

  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());

  // disable softAP
  WiFi.softAPdisconnect(true);
  server.close();

  // if the URL is known beforehand, the TLS fingerprint should be configured here instead
  client.setInsecure();
}

void loop()
{
  int buttonRead = digitalRead(BUTTON);

  if (buttonRead == HIGH)
  {
    digitalWrite(LED, LOW);
  }
  else
  {
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
      if (request.begin(client, url))
      {
        // Make HTTP request, we don't care about the response
        request.GET();
        // debounce
        delay(5000);
      }
      else
      {
        Serial.println("Request has failed, please check URL");
      }
    }
  }
}
