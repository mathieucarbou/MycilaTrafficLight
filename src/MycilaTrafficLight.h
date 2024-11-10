// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2024 Mathieu Carbou
 */
#pragma once

#include <WString.h>
#include <esp32-hal-gpio.h>

#ifdef MYCILA_JSON_SUPPORT
  #include <ArduinoJson.h>
#endif

#ifdef MYCILA_NEOPIXEL_SUPPORT
  #include <Adafruit_NeoPixel.h>
#endif

#define MYCILA_TRAFFIC_LIGHT_VERSION          "1.1.1"
#define MYCILA_TRAFFIC_LIGHT_VERSION_MAJOR    1
#define MYCILA_TRAFFIC_LIGHT_VERSION_MINOR    1
#define MYCILA_TRAFFIC_LIGHT_VERSION_REVISION 1

namespace Mycila {
  class TrafficLight {
    public:
      enum class State {
        NONE = 0,
        ON,
        OFF
      };

    public:
      bool begin(const int8_t greenPin, const int8_t yellowPin, const int8_t redPin);

#ifdef MYCILA_NEOPIXEL_SUPPORT
      bool begin(const int8_t neoPin, const size_t neoCount = 3, const neoPixelType neoType = NEO_GRB + NEO_KHZ800);
#endif

      void end();

      // indicates whether the traffic light is enabled with backed GPIO pins
      bool isEnabled() const;

      void setAllOn() { set(State::ON, State::ON, State::ON); }
      void setAllOff() { set(State::OFF, State::OFF, State::OFF); }

      void setGreen(bool state) { set(state ? State::ON : State::OFF, State::NONE, State::NONE); }
      void setYellow(bool state) { set(State::NONE, state ? State::ON : State::OFF, State::NONE); }
      void setRed(bool state) { set(State::NONE, State::NONE, state ? State::ON : State::OFF); }
      void set(State green, State yellow, State red);

      // set the brightness of the NeoPixel strip
      // No-op for GPIO pins
      void setBrightness(const float brightness);

      State getGreen() const { return _green ? State::ON : State::OFF; }
      State getYellow() const { return _yellow ? State::ON : State::OFF; }
      State getRed() const { return _red ? State::ON : State::OFF; }

      bool isGreenOn() const { return getGreen() == State::ON; }
      bool isYellowOn() const { return getYellow() == State::ON; }
      bool isRedOn() const { return getRed() == State::ON; }

      bool areAllOn() const { return isGreenOn() && isYellowOn() && isRedOn(); }
      bool areAllOff() const { return !isGreenOn() && !isYellowOn() && !isRedOn(); }

      String toString() const;

#ifdef MYCILA_JSON_SUPPORT
      void toJson(const JsonObject& root) const;
#endif

      gpio_num_t getGreenPin() const { return _greenPin; }
      gpio_num_t getYellowPin() const { return _yellowPin; }
      gpio_num_t getRedPin() const { return _redPin; }

    private:
      bool _green = false;
      bool _yellow = false;
      bool _red = false;

      bool _gpioEnabled = false;

      gpio_num_t _greenPin = GPIO_NUM_NC;
      gpio_num_t _yellowPin = GPIO_NUM_NC;
      gpio_num_t _redPin = GPIO_NUM_NC;

#ifdef MYCILA_NEOPIXEL_SUPPORT
      Adafruit_NeoPixel* _strip = nullptr;
#endif
  };
} // namespace Mycila
