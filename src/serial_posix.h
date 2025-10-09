#ifndef SERIAL_POSIX_H
#define SERIAL_POSIX_H

#include <stdint.h>
#include <termios.h>

typedef struct {
    int fd;
    struct termios oldtio;
} serial_port_t;

int  serial_open(serial_port_t* sp, const char* dev_path, int baud);
int  serial_close(serial_port_t* sp);
int  serial_write(serial_port_t* sp, const uint8_t* buf, int len);
/**
 * Lê até maxlen bytes com timeout em milissegundos.
 * Retorna quantidade lida (>=0) ou -1 em erro.
 */
int  serial_read (serial_port_t* sp, uint8_t* buf, int maxlen, int timeout_ms);

#endif
