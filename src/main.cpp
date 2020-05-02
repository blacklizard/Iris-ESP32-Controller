#define FASTLED_ALLOW_INTERRUPTS 0
#include "config.h"

#include <FastLED.h>
#include "WiFi.h"

#define SCREEN 1
#define STATIC 2

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

WiFiServer server(8080);
CRGB leds[NUM_LEDS];

uint8_t mode[1];
uint8_t color[3];
void setup()
{
  Serial.begin(115200);
  delay(1000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  Serial.print("Device IP: ");
  Serial.println(WiFi.localIP());

  server.begin();

  FastLED.addLeds<LED_CHIP, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(UncorrectedColor);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop()
{
  WiFiClient client = server.available();
  while (client.connected() && client.available())
  {
    client.read(&mode[0], 1);
    if (mode[0] == SCREEN)
    {
      client.read((uint8_t *)leds, NUM_LEDS * 3);
    }
    else if (mode[0] == STATIC)
    {
      client.read((uint8_t *)color, 3);
      for (size_t i = 0; i < NUM_LEDS; i++)
      {
        leds[i].red = color[0];
        leds[i].green = color[1];
        leds[i].blue = color[2];
      }
    }
    delay(1); // needed to avoid random flickering of the LED strip
    FastLED.show();
  }
}