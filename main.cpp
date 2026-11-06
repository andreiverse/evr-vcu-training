#include "inc/tasks.h"
#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  setupTask1x4();
  setupTask2();
  setupTask3();
  setupTask5x6();
}

void loop() {
  loopTask2();
  loopTask5x6();
}
