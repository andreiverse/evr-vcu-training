#include <Arduino.h>
#include "tasks.h"

void setup() {
  Serial.begin(9600);
  setupTask1x4();
  setupTask2();
  setupTask3();
  setupTask5();
}

void loop() {
  loopTask2();
  loopTask5();
}
