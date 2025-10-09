
#include "serial.h"
#include <stdio.h>

// Declare the serial port handle
serial_port_t sp;

int main(int argc, char *argv[]) {
    const char* dev = "/dev/ttyUSB0";
    int baud = 9600;

    if (serial_open(&sp, "/dev/ttyUSB0", 9600)!=0) {
        puts("Falha ao abrir /dev/ttyUSB0");
        return 1;
    }

    if (argc >= 2) dev = argv[1];
    if (argc >= 3) baud = atoi(argv[2]);

    if (serial_open(&sp, dev, baud)!=0){
        fprintf(stderr, "Falha ao abrir %s @ %d bps\n", dev, baud);
        return 1;
    }

    return 0;
}
