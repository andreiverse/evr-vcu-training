#include <Arduino.h>
#include "inc/i2c.h"

#define MA_ADC 50

uint16_t adc_values[MA_ADC] = {0};
uint8_t next_i = 0;

void adcMaInsert(uint16_t val)
{
  adc_values[next_i = (++next_i) % MA_ADC] = val;
}

uint16_t adcMaValue()
{
  uint32_t sum = 0;
  for (int i = 0; i < MA_ADC; i++)
  {
    sum += adc_values[i];
  }  
  
  return (uint16_t)ceil(1.0f * sum / MA_ADC);
}

void adcMaInit(uint16_t val)
{
    for (int i = 0; i < MA_ADC; i++)
    {
        adc_values[i] = val;
    }
}

void setupTask7()
{
    i2cSetup();
    adcMaInit(ADC);
}

void loopTask7()
{
    i2cLoop();
    adcMaInsert(ADC);

    uint32_t now_value = (uint64_t)(min(5000, (adcMaValue() / 1023.0f) * 5000));
    static uint32_t last_value = 0;

    int16_t diff = now_value - last_value;
    
    if (abs(diff) > 15 || last_value == 0) {
        i2cRegisterWrite(0x04, now_value / 1000);
        i2cRegisterWrite(0x03, (now_value / 100) % 10);
        i2cRegisterWrite(0x02, (now_value / 10) % 10);
        i2cRegisterWrite(0x01, now_value % 10);
        last_value = now_value;
    }
}