#include <stdint.h>

void blockForAbit()
{
  volatile uint32_t timer = 0;
  while (timer < 3000) timer++;
  timer = 0;
}