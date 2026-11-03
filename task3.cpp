#include <Arduino.h>

volatile static bool led_toggle = false;

// Task 3
// Digital 2 - button to gnd
// Digital 5 - led
void setupTask3()
{
  DDRD |= 1 << PD5;

  PORTD |= (1 << PORTD2);
  EICRA &= ~(1 << ISC00);
  EICRA |= 1 << ISC01;
  EIMSK |= 1 << INT0;
}

ISR(INT0_vect)
{
  Serial.println("Task 3: interrupt trigger");

  led_toggle = !led_toggle;

  if (led_toggle)
  {
    PORTD |= 1 << PD5;
  } else {
    PORTD &= ~(1 << PD5);
  }

}

