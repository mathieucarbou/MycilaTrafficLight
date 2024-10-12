#include <Arduino.h>
#include <MycilaTrafficLight.h>

// Which pin is connected to the NeoPixels?
constexpr uint16_t NEOPIXEL_PIN = 4;

// How many NeoPixels
constexpr uint16_t NEOPIXEL_COUNT = 3;

// Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
constexpr neoPixelType NEOPIXEL_FLAGS = NEO_GRB + NEO_KHZ800;

Mycila::TrafficLight lights;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    continue;

  // Argument 1 = pin number (most are valid)
  // Argument 2 = Number of pixels in NeoPixel strip (default 3 if omited)
  // Argument 3 = Pixel type flags defaut if omited ( NEO_GRB + NEO_KHZ800 )
  lights.begin(NEOPIXEL_PIN, NEOPIXEL_COUNT, NEOPIXEL_FLAGS);

  // Set BRIGHTNESS to about 1/5 (0.0 to 1.0)
  lights.setBrightness(0.2f);
  lights.setAllOff();
}

void loop() {

  // Simple Traffic Light
  lights.setGreen(true);
  delay(3000);

  lights.setAllOff();
  delay(200);

  lights.setYellow(true);
  delay(1000);

  lights.setAllOff();
  delay(200);

  lights.setRed(true);
  delay(3000);

  lights.setAllOff();
  delay(200);
}
