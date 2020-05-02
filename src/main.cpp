#define FASTLED_ALLOW_INTERRUPTS 0
#include "config.h"

#include <FastLED.h>
#include "WiFi.h"
 
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
 
WiFiServer server(8080);
CRGB leds[NUM_LEDS]; 
 
void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
  Serial.print("Device IP: ");
  Serial.println(WiFi.localIP());
 
  server.begin();

  FastLED.addLeds<LED_CHIP, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
}
 
void loop() {
  WiFiClient client = server.available();
  while (client.connected() && client.available())
  {
    client.read((uint8_t*)leds, NUM_LEDS*3);
    delay(1);// needed to avoid random flickering of the LED strip
    FastLED.show();
  }
}