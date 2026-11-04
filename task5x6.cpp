#include <Arduino.h>

uint8_t duty_cycle = 0;

// task 6: analog 3 (PC3)
void setupTask6Adc()
{
  ADMUX |= 1 << REFS0; // set reference to in 
  ADMUX |= (1 << MUX0 | 1 << MUX1); // read adc[3]

  ADCSRA |= 1 << ADEN; // enable adc
  ADCSRA |= 1 << ADATE; // auto enable adc

  ADCSRA |= (1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0); // /128 division factor

  ADCSRB = 0; // default is free running flow
  ADCSRA |= 1 << ADSC; // start first conversation
}

void setupTask5x6PWM()
{
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
  TCCR2A |= 1 << COM2A1;
  TCCR2A &= ~(1 << COM2A0);

//   OCR2A = 255;
  OCR2B = duty_cycle;
}

void setupTask5x6()
{
  // digital 3 = output (Task 5)
  DDRD |= 1 << DDD3;

  // digital 11 = output (Task 6)
  DDRB |= 1 << DDB3;
  
  setupTask5x6PWM();
  setupTask6Adc();
}

void loopTask5x6()
{
  static uint8_t direction = 1;
  static uint32_t last_updated_ms = 0;
  static volatile uint32_t timer = 0;

  // task 6
  OCR2A = (ADC / 1023.0f)*255;
  
  // task 5
  // if ((now_ms - last_updated_ms) > 5)
  // {
  duty_cycle += direction;
  
  OCR2B = duty_cycle;      

  if (duty_cycle >= 255)
    direction = -1;
  if (duty_cycle <= 0)
    direction = 1;

    // last_updated_ms = now_ms;
  // }

  while (timer < 5000) timer++;

  timer = 0;
}
