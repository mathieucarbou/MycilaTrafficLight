// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2024 Mathieu Carbou
 */
#include <MycilaTrafficLight.h>

#ifdef MYCILA_LOGGER_SUPPORT
  #include <MycilaLogger.h>
extern Mycila::Logger logger;
  #define LOGD(tag, format, ...) logger.debug(tag, format, ##__VA_ARGS__)
  #define LOGI(tag, format, ...) logger.info(tag, format, ##__VA_ARGS__)
  #define LOGW(tag, format, ...) logger.warn(tag, format, ##__VA_ARGS__)
  #define LOGE(tag, format, ...) logger.error(tag, format, ##__VA_ARGS__)
#else
  #define LOGD(tag, format, ...) ESP_LOGD(tag, format, ##__VA_ARGS__)
  #define LOGI(tag, format, ...) ESP_LOGI(tag, format, ##__VA_ARGS__)
  #define LOGW(tag, format, ...) ESP_LOGW(tag, format, ##__VA_ARGS__)
  #define LOGE(tag, format, ...) ESP_LOGE(tag, format, ##__VA_ARGS__)
#endif

#define TAG "LIGHTS"

#ifndef GPIO_IS_VALID_OUTPUT_GPIO
  #define GPIO_IS_VALID_OUTPUT_GPIO(gpio_num) ((gpio_num >= 0) && \
                                               (((1ULL << (gpio_num)) & SOC_GPIO_VALID_OUTPUT_GPIO_MASK) != 0))
#endif

bool Mycila::TrafficLight::begin(const int8_t greenPin, const int8_t yellowPin, const int8_t redPin) {
  if (_gpioEnabled) {
    LOGW(TAG, "Traffic light already enabled");
    return false;
  }

  if (GPIO_IS_VALID_OUTPUT_GPIO(greenPin)) {
    _greenPin = (gpio_num_t)greenPin;
    LOGI(TAG, "Enable Green LED on pin: %" PRId8, greenPin);
  } else {
    LOGE(TAG, "Invalid Green LED pin: %" PRId8, greenPin);
    _greenPin = GPIO_NUM_NC;
    return false;
  }

  if (GPIO_IS_VALID_OUTPUT_GPIO(yellowPin)) {
    _yellowPin = (gpio_num_t)yellowPin;
    LOGI(TAG, "Enable Yellow LED on pin: %" PRId8, yellowPin);
  } else {
    LOGE(TAG, "Invalid Yellow LED pin: %" PRId8, yellowPin);
    _greenPin = GPIO_NUM_NC;
    _yellowPin = GPIO_NUM_NC;
    return false;
  }

  if (GPIO_IS_VALID_OUTPUT_GPIO(redPin)) {
    _redPin = (gpio_num_t)redPin;
    LOGI(TAG, "Enable Red LED on pin: %" PRId8, redPin);
  } else {
    LOGE(TAG, "Invalid Red LED pin: %" PRId8, redPin);
    _greenPin = GPIO_NUM_NC;
    _yellowPin = GPIO_NUM_NC;
    _redPin = GPIO_NUM_NC;
    return false;
  }

  pinMode(_greenPin, OUTPUT);
  pinMode(_yellowPin, OUTPUT);
  pinMode(_redPin, OUTPUT);

  _gpioEnabled = true;
  return true;
}

#ifdef MYCILA_NEOPIXEL_SUPPORT
bool Mycila::TrafficLight::begin(const int8_t neoPin, const size_t neoCount, const neoPixelType neoType) {
  if (_strip != nullptr) {
    LOGW(TAG, "Traffic light already enabled");
    return false;
  }

  if (!GPIO_IS_VALID_OUTPUT_GPIO(neoPin)) {
    LOGE(TAG, "Invalid NeoPixel LED pin: %" PRId8, neoPin);
    return false;
  }

  if (neoCount < 3) {
    LOGE(TAG, "Not Enough NeoPixel LED count: %" PRId16, neoCount);
    return false;
  }

  LOGI(TAG, "Enable %" PRId16 " NeoPixel LED on pin: %" PRId8, neoCount, neoPin);
  _strip = new Adafruit_NeoPixel(neoCount, neoPin, neoType);
  _strip->begin();

  return true;
}
#endif

void Mycila::TrafficLight::end() {
  set(State::OFF, State::OFF, State::OFF);

  if (_gpioEnabled) {
    digitalWrite(_greenPin, LOW);
    digitalWrite(_yellowPin, LOW);
    digitalWrite(_redPin, LOW);

    _greenPin = GPIO_NUM_NC;
    _yellowPin = GPIO_NUM_NC;
    _redPin = GPIO_NUM_NC;
  }

#ifdef MYCILA_NEOPIXEL_SUPPORT
  if (_strip != nullptr) {
    digitalWrite(_strip->getPin(), LOW);
    _strip->setBrightness(0);
    delete _strip;
    _strip = nullptr;
  }
#endif
}

bool Mycila::TrafficLight::isEnabled() const {
#ifdef MYCILA_NEOPIXEL_SUPPORT
  return _gpioEnabled || _strip != nullptr;
#else
  return _gpioEnabled;
#endif
}

void Mycila::TrafficLight::set(State green, State yellow, State red) {
  if (green != State::NONE)
    _green = green == State::ON;
  if (yellow != State::NONE)
    _yellow = yellow == State::ON;
  if (red != State::NONE)
    _red = red == State::ON;

  if (_gpioEnabled) {
    digitalWrite(_greenPin, _green ? HIGH : LOW);
    digitalWrite(_yellowPin, _yellow ? HIGH : LOW);
    digitalWrite(_redPin, _red ? HIGH : LOW);
  }

#ifdef MYCILA_NEOPIXEL_SUPPORT
  if (_strip != nullptr) {
    _strip->setPixelColor(0, _green ? _strip->Color(0, 255, 0) : _strip->Color(0, 0, 0));
    _strip->setPixelColor(1, _yellow ? _strip->Color(128, 128, 0) : _strip->Color(0, 0, 0));
    _strip->setPixelColor(2, _red ? _strip->Color(255, 0, 0) : _strip->Color(0, 0, 0));
    _strip->show();
  }
#endif
}

void Mycila::TrafficLight::setBrightness(const float brightness) {
#ifdef MYCILA_NEOPIXEL_SUPPORT
  if (_strip != nullptr) {
    _strip->setBrightness(static_cast<uint8_t>(brightness * 255));
  }
#endif
}

#ifdef MYCILA_JSON_SUPPORT
void Mycila::TrafficLight::toJson(const JsonObject& root) const {
  root["code"] = toString();
  root["green"] = _green;
  root["red"] = _red;
  root["yellow"] = _yellow;
}
#endif
