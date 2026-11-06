#include <Arduino.h>
#include <avr/io.h>

int i = 0;

//   Task 1 & 4
// Sequencial lighting 3 leds
// Analog 0, 1, 2 - leds
// Timer1
void setupTask1x4() {
  TCCR1A = 0;
  TCCR1B = 0;

  // CTC mode (clear timer on compare match)
  TCCR1B |= (1 << WGM12);

  // set prescaler to 64
  TCCR1B |= (1 << CS11 | 1 << CS10);
  // f = 16000000hz / 64 = 250000hz (T = 1 / f = 0.000004s)
  // we need 0.250ms so 0.250s/0.000004s = 62500
  OCR1A = 62500;

  // enable COMPA interrupt
  TIMSK1 |= 1 << OCIE2A;

  // set output on the 3 leds
  DDRC |= 1 << DDC0;
  DDRC |= 1 << DDC1;
  DDRC |= 1 << DDC2;
}

void cycleLeds() {
  i = (i + 1) % 4;

  PORTC &= ~(1 << DD0);
  PORTC &= ~(1 << DD1);
  PORTC &= ~(1 << DD2);

  if (i >= 1)
    PORTC |= 1 << DD0;
  if (i >= 2)
    PORTC |= 1 << DD1;
  if (i >= 3)
    PORTC |= 1 << DD2;
}

ISR(TIMER1_COMPA_vect) { cycleLeds(); }
