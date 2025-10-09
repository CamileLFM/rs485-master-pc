#include "utils.h"
#include <stdio.h>
#include <time.h>

void hex_dump(const char* prefix, const uint8_t* b, int n){
    if(prefix) printf("%s ", prefix);
    for (int i=0;i<n;i++) printf("%02X ", (unsigned)b[i]);
    puts("");
}

float to_celsius_from_u16(uint16_t raw){
    return ((float)raw)/10.0f;
}

uint16_t u16_le(const uint8_t lo, const uint8_t hi){
    return (uint16_t)lo | ((uint16_t)hi<<8);
}

void sleep_ms(unsigned ms){
    struct timespec ts;
    ts.tv_sec  = ms/1000;
    ts.tv_nsec = (ms%1000)*1000000L;
    nanosleep(&ts, NULL);
}
