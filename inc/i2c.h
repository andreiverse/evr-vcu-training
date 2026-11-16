
static bool waitTwint();
bool i2cLog(uint8_t expected, const char *message);
bool start();
bool stop();
bool sendByte(uint8_t b, const char *msg);
bool addressW();
bool registerWrite(uint8_t reg, uint8_t value);
bool setupTwi();
void loopI2c();
void setupI2c();