#include "inc/utils.h"
#include <Arduino.h>

uint8_t duty_cycle = 0;

// task 6: analog 3 (PC3)
void setupTask6Adc() {
  ADMUX |= 1 << REFS0;              // set reference to Vin
  ADMUX |= (1 << MUX0 | 1 << MUX1); // read adc[3]

  ADCSRA |= 1 << ADEN;  // enable adc
  ADCSRA |= 1 << ADATE; // auto enable adc

  ADCSRA |= (1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0); // /128 division factor

  ADCSRB = 0;          // default is free running flow
  ADCSRA |= 1 << ADSC; // start first conversation
}

void setupTask5x6PWM() {
  TCCR2A = 0;
  TCCR2B = 0;

  // /64 prescaler
  TCCR2B |= 1 << CS22;

  // enable fast pwm mode
  TCCR2A |= (1 << WGM21 | 1 << WGM20);

  // enable OSC B non-inverting mode (pin)
  TCCR2A |= 1 << COM2B1;
  TCCR2A &= ~(1 << COM2B0);

  // enable OSC A non-inverting mode
  TCCR2A |= 1 << COM2A1;
  TCCR2A &= ~(1 << COM2A0);

  OCR2A = 0;          // PB3 (digital 11)
  OCR2B = duty_cycle; // PD3 (digital 3)
}

void setupTask5x6() {
  // digital 3 = output (Task 5)
  DDRD |= 1 << DDD3;
  // digital 11 = output (Task 6)
  DDRB |= 1 << DDB3;

  setupTask5x6PWM();
  setupTask6Adc();
}

void loopTask5x6() {
  constexpr uint8_t TOP = 20;
  constexpr uint8_t BOT = 0;

  static int8_t direction = 1;

  // Task 6
  OCR2A = (ADC / 1023.0f) * 255;

  // Task 5
  if (OCR2B >= TOP)
    direction = -1;
  else if (OCR2B <= BOT)
    direction = 1;

  OCR2B += direction;

}