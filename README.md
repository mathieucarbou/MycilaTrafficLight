# MycilaTrafficLight

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Continuous Integration](https://github.com/mathieucarbou/MycilaTrafficLight/actions/workflows/ci.yml/badge.svg)](https://github.com/mathieucarbou/MycilaTrafficLight/actions/workflows/ci.yml)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/mathieucarbou/library/MycilaTrafficLight.svg)](https://registry.platformio.org/libraries/mathieucarbou/MycilaTrafficLight)

ESP32 / Arduino Library for Traffic Light LEDs

- Supports standard GPIO leds
- Supports NeoPixel leds (addition from [@hallard](https://github.com/hallard))

**NeoPixel example:**

```cpp
// Argument 1 = pin number (most are valid)
// Argument 2 = Number of pixels in NeoPixel strip (default 3 if omited)
// Argument 3 = Pixel type flags defaut if omited ( NEO_GRB + NEO_KHZ800 )
lights.begin(NEOPIXEL_PIN, NEOPIXEL_COUNT, NEOPIXEL_FLAGS);

// Set BRIGHTNESS to about 1/5 (0.0 to 1.0)
lights.setBrightness(0.2f);

lights.setAllOff();
lights.setGreen(true);
lights.setYellow(true);
lights.setRed(true);
// [...]
```
