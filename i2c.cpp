#include <Arduino.h>
#include "inc/utils.h"

uint8_t is_twi_ok = 0;

#define I2C_SLAVE_ADDRESS 0b0000000
// #define DEBUG_I2c

uint8_t getI2cStatus()
{
  return TWSR & 0xF8;
}

bool i2cWaitTWINT()
{
  uint32_t t0 = 0;
  while (!(TWCR & (1 << TWINT)))
  {
    t0++;
    if (t0 > 5000)
    {
      #ifdef DEBUG_I2c
      Serial.println(F("FAIL: TWINT timeout"));
      #endif
      is_twi_ok = 0;
      return false;
    }
  }
  return true;
}


bool i2cVerifyStatus(uint8_t expected, const char *message)
{
  uint8_t status = getI2cStatus();

  if (status != expected)
  {
    #ifdef DEBUG_I2c
    Serial.print(F("FAIL "));
    Serial.print(message);
    Serial.print(F(": expected 0x"));
    Serial.print(expected, HEX);
    Serial.print(F(", got 0x"));
    Serial.println(status, HEX);
    #endif

    is_twi_ok = 0;
    return false;
  }

  return true;
}

bool i2cStart()
{
  if (!is_twi_ok) return 0;

  TWCR = (1 << TWEN | 1 << TWINT | 1 << TWSTA); // Enable TWI, reset TWINT and do start

  return i2cWaitTWINT() && i2cVerifyStatus(0x08, "start");
}

bool i2cStop()
{
  TWCR = (1 << TWEN | 1 << TWINT | 1 << TWSTO); // Enable TWI, reset TWINT and do stop
  uint32_t t0 = 0;
  while (TWCR & (1 << TWSTO))
  {
    t0++;
    if (t0 > 5000) { is_twi_ok = 0; return false; }
  }
  return true;
}

bool i2cSendByte(uint8_t b, const char *msg)
{
  if (!is_twi_ok) return 0;
  TWDR = b;
  TWCR = (1 << TWEN) | (1 << TWINT);
  return i2cWaitTWINT() && i2cVerifyStatus(0x28, msg);
}

bool i2cAddressW()
{
  if (!is_twi_ok) return 0;

  TWDR = I2C_SLAVE_ADDRESS << 1;
  TWCR = (1 << TWEN | 1 << TWINT); // send
  return i2cWaitTWINT() && i2cVerifyStatus(0x18, "SLA/W");
}

bool i2cRegisterWrite(uint8_t reg, uint8_t value)
{
  if (!i2cStart())
  {
    i2cStop();
    return 0;
  }

  if (!i2cAddressW())
  {
    i2cStop();
    return 0;
  } 
  
  if (!i2cSendByte(reg, "reg address"))
  {
    i2cStop();
    return 0;
  }

  if (!i2cSendByte(value, "reg data"))
  {
    i2cStop();
    return 0;
  }

  i2cStop();
  return 1;
}

bool i2cSetupDriver()
{
  return i2cRegisterWrite(0x0C, 0x01) && //shutdown
  i2cRegisterWrite(0x09, 0x3F) && // decode
  i2cRegisterWrite(0x0A, 0x08) && // global intesity
  i2cRegisterWrite(0x0B, 0x03) && // scan limit
  i2cRegisterWrite(0x0F, 0x00); // test mode
}

void i2cSetup()
{
  // 100Khz
  TWSR &= ~(1 << TWPS1 | 1 << TWPS0); // Prescaler = 1
  TWBR = 72;                          // Baud
}

void i2cLoop()
{
  if (!is_twi_ok)
  {
    TWCR = 0;

    is_twi_ok = 1;
    #ifdef DEBUG_I2c
    Serial.println(F("twi is not ok, setting up..."));
    #endif
    is_twi_ok = i2cSetupDriver();
    
    #ifdef DEBUG_I2c
    if (is_twi_ok) 
    {
      Serial.println(F("setup twi success"));
    } else
    {
      Serial.println(F("setup twi failure"));
    }
    #endif

    blockForAbit();
  }

}