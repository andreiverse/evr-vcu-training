#include <Arduino.h>

int i = 0;
volatile uint32_t timer = 100000;

bool task2led1 = false;
bool task2lastpress = false;

// unsigned uint16_t task4duty = 0;

ISR(TIMER2_COMPA_vect)
{
  OCR2A += 250;

  // task4duty += 1;
  // task4duty %= 100;
}

volatile bool task3led = false;

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

  task3led = !task3led;

  if (task3led)
  {
    PORTD |= 1 << PD5;
  } else {
    PORTD &= ~(1 << PD5);
  }

}

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

  if (pressed == 0 && task2lastpress == 1)
  {
    Serial.println("Pressed!");
    task2led1 = !task2led1;
  }

  task2lastpress = pressed;

  PORTD &= ~(1 << PD7);
  PORTD &= ~(1 << PD6);

  if (task2led1)
  {
    PORTD |= 1 << PD7;
  } else
  {
    PORTD |= 1 << PD6;
  }
}

uint8_t dutyCycleTask5 = 0;

void setupTask5()
{
  // digital 3 = output
  DDRD |= 1 << PD3;
  PORTD |= 1 << PD3;

  TCCR2A = 0;
  TCCR2B = 0;

  TCCR2B |= 1 << CS22; // prescaler 64
  
  // enable fast pwm mode
  TCCR2A |= 1 << WGM21;
  TCCR2A |= 1 << WGM20; 
  
  // non-inverting mode
  TCCR2A |= 1 << COM2B1;
  TCCR2A &= ~(1 << COM2B0);

  OCR2A = 249;
  OCR2B = dutyCycleTask5;
}


void loopTask5()
{
  uint32_t now = millis();
  static uint8_t direction = 1;
  static uint32_t lastUpdateTask5 = 0;

  if ((now - lastUpdateTask5) > 5)
  {
    dutyCycleTask5 += direction;
    OCR2B = dutyCycleTask5;

    if (dutyCycleTask5 >= 230)
      direction = -1;
    if (dutyCycleTask5 <= 10)
      direction = 1;

    lastUpdateTask5 = now;
  }

}

// Task 1 & 4
// Analog 0, 1, 2 - leds
void setupTask1x4()
{
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= B00000011;  // prescaler 64
  OCR1A = 62500;        // timer COM1A Register
  TIMSK1 |= B00000010;  // enable COMPA

  DDRC |= 1 << DD0;
  DDRC |= 1 << DD1;
  DDRC |= 1 << DD2;
}


void cycleLeds()
{
  i = (i + 1) % 5;
  
  PORTC &= ~(1 << DD0);
  PORTC &= ~(1 << DD1);
  PORTC &= ~(1 << DD2);

  if (i >= 1)
  {
    PORTC |= 1 << DD0;
  }
  if (i >= 2) {
    PORTC |= 1 << DD1;
  }
  if (i >= 3) {
    PORTC |= 1 << DD2;
  }
}

ISR(TIMER1_COMPA_vect)
{
  OCR1A += 62500;
  cycleLeds();
}


void loopTask1x4()
{
}


void setup() {
  Serial.begin(9600);
  setupTask1x4();
  setupTask2();
  setupTask3();
  setupTask5();
}

void loop() {
  loopTask1x4();
  loopTask2();
  loopTask5();
}
