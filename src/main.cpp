#define FASTLED_ALLOW_INTERRUPTS 0

#include "config.h"

#include <FastLED.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#define LED_PIN 14
#define NUM_LEDS 169
#define BRIGHTNESS 90
#define COLOR_ORDER GRB

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

WebServer server(8080);
CRGB leds[NUM_LEDS];

void handleSelect()
{
  int length = server.arg("color").length();
  char body[length + 1];
  server.arg("color").toCharArray(body, length + 1);

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = strtol(body, NULL, 16);
  }

  FastLED.show();
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Connection", "close");
  server.send(200, "text/plain", "ok");
}

void handleLed()
{
  int length = server.arg("colors").length();
  char body[length + 1];
  server.arg("colors").toCharArray(body, length + 1);
  char *chars_array = strtok(body, ":");
  int led = 0;
  while (chars_array)
  {
    long num = strtol(chars_array, NULL, 16);
    leds[led] = num;
    chars_array = strtok(NULL, ":");
    led++;
  }

  delete[] chars_array;
  FastLED.show();
  server.sendHeader("Connection", "close");
  server.send(200, "text/plain", "");
}

void setup(void)
{
  delay(500);
  FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(UncorrectedColor);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleLed);
  server.on("/select", handleSelect);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void)
{
  server.handleClient();
  delay(3);
}