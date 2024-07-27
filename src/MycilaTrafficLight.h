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

#define MYCILA_TRAFFIC_LIGHT_VERSION "1.0.0"
#define MYCILA_TRAFFIC_LIGHT_VERSION_MAJOR 1
#define MYCILA_TRAFFIC_LIGHT_VERSION_MINOR 0
#define MYCILA_TRAFFIC_LIGHT_VERSION_REVISION 0

namespace Mycila {
  class TrafficLight {
    public:
      enum class State {
        NONE = 0,
        ON,
        OFF
      };

    public:
      void begin(const int8_t greenPin, const int8_t yellowPin, const int8_t redPin);
      void end();

      bool isEnabled() const { return _greenPin != GPIO_NUM_NC || _yellowPin != GPIO_NUM_NC || _redPin != GPIO_NUM_NC; }

      inline void setAllOn() { set(State::ON, State::ON, State::ON); }
      inline void setAllOff() { set(State::OFF, State::OFF, State::OFF); }

      inline void setGreen(bool state) { set(state ? State::ON : State::OFF, State::NONE, State::NONE); }
      inline void setYellow(bool state) { set(State::NONE, state ? State::ON : State::OFF, State::NONE); }
      inline void setRed(bool state) { set(State::NONE, State::NONE, state ? State::ON : State::OFF); }
      void set(State green, State yellow, State red);

      State getGreen() const { return _green ? State::ON : State::OFF; }
      State getYellow() const { return _yellow ? State::ON : State::OFF; }
      State getRed() const { return _red ? State::ON : State::OFF; }

      inline bool isGreenOn() const { return getGreen() == State::ON; }
      inline bool isYellowOn() const { return getYellow() == State::ON; }
      inline bool isRedOn() const { return getRed() == State::ON; }

      inline bool areAllOn() const { return isGreenOn() && isYellowOn() && isRedOn(); }
      inline bool areAllOff() const { return !isGreenOn() && !isYellowOn() && !isRedOn(); }

      inline String toString() const { return String(isGreenOn() ? "🟢 " : "⚫ ") + (isYellowOn() ? "🟡 " : "⚫ ") + (isRedOn() ? "🔴" : "⚫"); }

#ifdef MYCILA_JSON_SUPPORT
      void toJson(const JsonObject& root) const;
#endif

      gpio_num_t getGreenPin() const { return _greenPin; }
      gpio_num_t getYellowPin() const { return _yellowPin; }
      gpio_num_t getRedPin() const { return _redPin; }

    private:
      gpio_num_t _greenPin = GPIO_NUM_NC;
      gpio_num_t _yellowPin = GPIO_NUM_NC;
      gpio_num_t _redPin = GPIO_NUM_NC;
      bool _green = false;
      bool _yellow = false;
      bool _red = false;
  };
} // namespace Mycila
