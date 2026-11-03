#include <Arduino.h>

bool led_toggle = false;
bool pressed_previous = false;

// Task 2
// Digital 7, 6 - leds
// Digial 0 - button to gnd

void setupTask2()
{
  DDRD &= ~(1 << DD0);
  PORTD |= (1 << PORTD0);

  DDRD |= 1 << PD7;
  DDRD |= 1 << PD6;
}

void loopTask2()
{
  bool pressed = (PIND & (1 << PIND0)) == 0;  

  if (pressed == 0 && pressed_previous == 1)
  {
    Serial.println("Pressed!");
    led_toggle = !led_toggle;
  }

  pressed_previous = pressed;

  PORTD &= ~(1 << PD7);
  PORTD &= ~(1 << PD6);

  if (led_toggle)
  {
    PORTD |= 1 << PD7;
  } else
  {
    PORTD |= 1 << PD6;
  }
}
