#include <Arduino.h>

int i = 0;

// Task 1 & 4
// Analog 0, 1, 2 - leds
void setupTask1x4()
{
  // init Timer/Counter1
  TCCR1A = 0;
  TCCR1B = 0;
  
  TCCR1B |= 11;  // prescaler 64
  TCCR1B |= (1 << WGM12) | (1 << CS21);
  OCR1A = 62500;        // timer COM1A Register
  TIMSK1 |= 1 << OCIE2A;  // enable COMPA

  DDRC |= 1 << DD0;
  DDRC |= 1 << DD1;
  DDRC |= 1 << DD2;
}

void cycleLeds()
{
  i = (i + 1) % 4;
  
  PORTC &= ~(1 << DD0);
  PORTC &= ~(1 << DD1);
  PORTC &= ~(1 << DD2);

  if (i >= 1) PORTC |= 1 << DD0;
  if (i >= 2) PORTC |= 1 << DD1;
  if (i >= 3) PORTC |= 1 << DD2;
}

ISR(TIMER1_COMPA_vect)
{
  cycleLeds();
}

