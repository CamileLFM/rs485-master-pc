#ifndef UTILS_H
#define UTILS_H
#include <stdint.h>
void hex_dump(const char* prefix, const uint8_t* b, int n);
float to_celsius_from_u16(uint16_t raw);
uint16_t u16_le(const uint8_t lo, const uint8_t hi);
void sleep_ms(unsigned ms);
#endif
