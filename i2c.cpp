#include <Arduino.h>
#include "inc/utils.h"

uint8_t is_twi_ok = 0;


uint8_t getI2cStatus()
{
  return TWSR & 0xF8;
}

static bool waitTwint()
{
  uint32_t t0 = micros();
  while (!(TWCR & (1 << TWINT)))
  {
    if (micros() - t0 > 2000)
    {
      Serial.println(F("FAIL: TWINT timeout"));
      is_twi_ok = 0;
      return false;
    }
  }
  return true;
}


bool i2cLog(uint8_t expected, const char *message)
{
  uint8_t status = getI2cStatus();

  if (status != expected)
  {
    Serial.print(F("FAIL "));
    Serial.print(message);
    Serial.print(F(": expected 0x"));
    Serial.print(expected, HEX);
    Serial.print(F(", got 0x"));
    Serial.println(status, HEX);

    is_twi_ok = 0;
    return false;
  }

  return true;
}

bool start()
{
  if (!is_twi_ok) return 0;

  TWCR = (1 << TWEN | 1 << TWINT | 1 << TWSTA); // Enable TWI, reset TWINT and do start

  return waitTwint() && i2cLog(0x08, "start");
}

bool stop()
{
  TWCR = (1 << TWEN | 1 << TWINT | 1 << TWSTO); // Enable TWI, reset TWINT and do stop
  uint32_t t0 = micros();
  while (TWCR & (1 << TWSTO))
  {
    if (micros() - t0 > 2000) { is_twi_ok = 0; return false; }
  }
  return true;
}

bool sendByte(uint8_t b, const char *msg)
{
  if (!is_twi_ok) return 0;
  TWDR = b;
  TWCR = (1 << TWEN) | (1 << TWINT);
  return waitTwint() && i2cLog(0x28, msg);
}

bool addressW()
{
  if (!is_twi_ok) return 0;

  TWDR = 0b00000000;
  TWCR = (1 << TWEN | 1 << TWINT); // send
  return waitTwint() && i2cLog(0x18, "SLA/W");
}

bool registerWrite(uint8_t reg, uint8_t value)
{
  if (!start())
  {
    stop();
    return 0;
  }

  if (!addressW())
  {
    stop();
    return 0;
  } 
  
  if (!sendByte(reg, "reg address"))
  {
    stop();
    return 0;
  }

  if (!sendByte(value, "reg data"))
  {
    stop();
    return 0;
  }

  stop();
  return 1;
}

bool setupTwi()
{
  return registerWrite(0x0C, 0x01) && //shutdown
  registerWrite(0x09, 0x3F) && // decode
  registerWrite(0x0A, 0x08) && // global intesity
  registerWrite(0x0B, 0x03) && // scan limit
  registerWrite(0x0F, 0x00); // test mode
}

void setupI2c()
{
  // 100Khz
  TWSR &= ~(1 << TWPS1 | 1 << TWPS0); // Prescaler = 1
  TWBR = 72;                          // Baud
}

void loopI2c()
{
  if (!is_twi_ok)
  {
    TWCR = 0;

    is_twi_ok = 1;
    Serial.println("twi is not ok, setting up...");
    blockForAbit();
    is_twi_ok = setupTwi();
    if (is_twi_ok) 
    {
      Serial.println("setup twi success");
    } else
    {
      Serial.println("setup twi failure");
    }
    blockForAbit();
  }

}