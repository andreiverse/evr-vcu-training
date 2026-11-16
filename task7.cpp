#include <Arduino.h>
#include "inc/i2c.h"

#define MA_ADC 32

uint16_t adc_values[MA_ADC] = {0};
uint8_t next_i = 0;

void insert_adc(uint16_t val)
{
  adc_values[((next_i++) % MA_ADC)] = val;
}

uint16_t average()
{
  uint32_t sum = 0;
  for (int i = 0; i < MA_ADC; i++)
  {
    sum += adc_values[i];
  }  
  
  return (uint16_t)ceil(1.0f * sum / MA_ADC);
}


void setupTask7()
{
    setupI2c();
}

void loopTask7()
{
    loopI2c();

    static uint64_t last_update = 0;
    uint64_t now = millis();

    insert_adc(ADC);

    uint64_t now_value = (uint64_t)(min(5000, (average() / 1023.0f) * 5000));
    static uint64_t last_value = 0;

    int16_t diff = now_value - last_value;
    if (
        ((now - last_update) > 20) && (abs(diff) > 10))
    {
        registerWrite(0x04, now_value / 1000);
        registerWrite(0x03, (now_value / 100) % 10);
        registerWrite(0x02, (now_value / 10) % 10);
        registerWrite(0x01, now_value % 10);
        last_update = now;
        last_value = now_value;
    }
}