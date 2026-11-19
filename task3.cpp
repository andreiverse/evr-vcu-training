#include <Arduino.h>

// Task 3
// Digital 2 - button to gnd
// Digital 5 - led
void setupTask3() {
  DDRD |= 1 << PD5;
  PORTD |= (1 << PORTD2);

  // falling edge detection on int0
  EICRA &= ~(1 << ISC00);
  EICRA |= 1 << ISC01;

  // enable int0
  EIMSK |= 1 << INT0;
}

ISR(INT0_vect) { PORTD ^= 1 << PORTD5; }
