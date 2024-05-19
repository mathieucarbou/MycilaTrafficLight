// SPDX-License-Identifier: MIT
/*
 * Copyright (C) 2023-2024 Mathieu Carbou and others
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

void Mycila::TrafficLight::begin(const int8_t greenPin, const int8_t yellowPin, const int8_t redPin) {
  if (GPIO_IS_VALID_OUTPUT_GPIO(greenPin)) {
    _greenPin = (gpio_num_t)greenPin;
    LOGI(TAG, "Enable Green LED on pin: %" PRId8, greenPin);
    pinMode(_greenPin, OUTPUT);
  } else {
    LOGE(TAG, "Invalid Green LED pin: %" PRId8, greenPin);
    _greenPin = GPIO_NUM_NC;
  }

  if (GPIO_IS_VALID_OUTPUT_GPIO(yellowPin)) {
    _yellowPin = (gpio_num_t)yellowPin;
    LOGI(TAG, "Enable Yellow LED on pin: %" PRId8, yellowPin);
    pinMode(_yellowPin, OUTPUT);
  } else {
    LOGE(TAG, "Invalid Yellow LED pin: %" PRId8, yellowPin);
    _yellowPin = GPIO_NUM_NC;
  }

  if (GPIO_IS_VALID_OUTPUT_GPIO(redPin)) {
    _redPin = (gpio_num_t)redPin;
    LOGI(TAG, "Enable Red LED on pin: %" PRId8, redPin);
    pinMode(_redPin, OUTPUT);
  } else {
    LOGE(TAG, "Invalid Red LED pin: %" PRId8, redPin);
    _redPin = GPIO_NUM_NC;
  }
}

void Mycila::TrafficLight::end() {
  set(State::OFF, State::OFF, State::OFF);
  if (_greenPin != GPIO_NUM_NC)
    digitalWrite(_greenPin, LOW);
  if (_yellowPin != GPIO_NUM_NC)
    digitalWrite(_yellowPin, LOW);
  if (_redPin != GPIO_NUM_NC)
    digitalWrite(_redPin, LOW);
  _greenPin = GPIO_NUM_NC;
  _yellowPin = GPIO_NUM_NC;
  _redPin = GPIO_NUM_NC;
}

void Mycila::TrafficLight::set(State green, State yellow, State red) {
  if (green != State::NONE)
    _green = green == State::ON;
  if (yellow != State::NONE)
    _yellow = yellow == State::ON;
  if (red != State::NONE)
    _red = red == State::ON;
  if (_greenPin != GPIO_NUM_NC)
    digitalWrite(_greenPin, _green ? HIGH : LOW);
  if (_yellowPin != GPIO_NUM_NC)
    digitalWrite(_yellowPin, _yellow ? HIGH : LOW);
  if (_redPin != GPIO_NUM_NC)
    digitalWrite(_redPin, _red ? HIGH : LOW);
}

#ifdef MYCILA_JSON_SUPPORT
void Mycila::TrafficLight::toJson(const JsonObject& root) const {
  root["code"] = toString();
  root["green"] = _green;
  root["red"] = _red;
  root["yellow"] = _yellow;
}
#endif
