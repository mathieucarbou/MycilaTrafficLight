#include <Arduino.h>
#include <ArduinoJson.h>
#include <MycilaTrafficLight.h>

Mycila::TrafficLight lights;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    continue;

  lights.setAllOn();

  JsonDocument doc;
  lights.toJson(doc.to<JsonObject>());
  serializeJson(doc, Serial);
  Serial.println();
}

void loop() {
  vTaskDelete(NULL);
}
