#include <Arduino.h>

uint8_t duty_cycle = 0;

void setupTask5()
{
  // digital 3 = output
  DDRD |= 1 << DDD3;

  // DDRB |= 1 << DDB3; pin digital 11
  
  TCCR2A = 0;
  TCCR2B = 0;

  TCCR2B |= 1 << CS22; // prescaler 64
  
  // enable fast pwm mode
  TCCR2A |= 1 << WGM21;
  TCCR2A |= 1 << WGM20; 
  
  // enable OSC B non-inverting mode
  TCCR2A |= 1 << COM2B1;
  TCCR2A &= ~(1 << COM2B0);
  
  // enable OSC A non-inverting mode
  // TCCR2A |= 1 << COM2A1;
  // TCCR2A &= ~(1 << COM2A0);

  OCR2A = duty_cycle;
  OCR2B = duty_cycle;
}


void loopTask5()
{
  uint32_t now_ms = millis();
  static uint8_t direction = 1;
  static uint32_t last_updated_ms = 0;

  if ((now_ms - last_updated_ms) > 5)
  {
    duty_cycle += direction;
    OCR2B = duty_cycle;
    OCR2A = duty_cycle;

    if (duty_cycle >= 230)
      direction = -1;
    if (duty_cycle <= 10)
      direction = 1;

    last_updated_ms = now_ms;
  }

}
