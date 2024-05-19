#include <Arduino.h>
#include <MycilaTrafficLight.h>

Mycila::TrafficLight lights;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    continue;

  lights.setAllOn();                             // current won't pass
}

void loop() {
}
