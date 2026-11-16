#include <Arduino.h>

static bool i2cWaitTWINT();
bool i2cVerifyStatus(uint8_t expected, const char *message);
bool i2cStart();
bool i2cStop();
bool i2cSendByte(uint8_t b, const char *msg);
bool i2cAddressW();
bool i2cRegisterWrite(uint8_t reg, uint8_t value);
bool i2cSetupDriver();
void i2cLoop();
void i2cSetup();