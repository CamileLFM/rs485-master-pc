#include "serial_posix.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <errno.h>
#include <asm-generic/termbits-common.h>

static speed_t to_baud(int baud){
    switch(baud){
        case 9600:   return B9600;
        case 19200:  return B19200;
        case 38400:  return B38400;
        case 57600:  return B57600;
        case 115200: return B115200;
        default:     return B9600;
    }
}

int serial_open(serial_port_t* sp, const char* dev_path, int baud){
    memset(sp, 0, sizeof(*sp));
    sp->fd = open(dev_path, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (sp->fd < 0) return -1;

    struct termios tio;
    if (tcgetattr(sp->fd, &sp->oldtio) < 0) { close(sp->fd); return -2; }
    memset(&tio, 0, sizeof(tio));

    cfsetispeed(&tio, to_baud(baud));
    cfsetospeed(&tio, to_baud(baud));

    // 8N1, sem controle de fluxo, raw mode
    tio.c_cflag |= (CLOCAL | CREAD);
    tio.c_cflag &= ~PARENB;
    tio.c_cflag &= ~CSTOPB;
    tio.c_cflag &= ~CSIZE;
    tio.c_cflag |= CS8;
    tio.c_cflag &= ~CRTSCTS;

    tio.c_lflag = 0;
    tio.c_iflag = 0;
    tio.c_oflag = 0;

    // Desabilita traduções e timeouts da linha
    tio.c_cc[VMIN]  = 0;
    tio.c_cc[VTIME] = 0;

    if (tcflush(sp->fd, TCIOFLUSH) < 0) { close(sp->fd); return -3; }
    if (tcsetattr(sp->fd, TCSANOW, &tio) < 0) { close(sp->fd); return -4; }

    // Modo bloqueante após configurar
    int flags = fcntl(sp->fd, F_GETFL, 0);
    fcntl(sp->fd, F_SETFL, flags & ~O_NONBLOCK);

    return 0;
}

int serial_close(serial_port_t* sp){
    if (sp->fd >= 0){
        tcsetattr(sp->fd, TCSANOW, &sp->oldtio);
        return close(sp->fd);
    }
    return 0;
}

int serial_write(serial_port_t* sp, const uint8_t* buf, int len){
    int w = (int)write(sp->fd, buf, len);
    return (w == len) ? w : -1;
}

int serial_read(serial_port_t* sp, uint8_t* buf, int maxlen, int timeout_ms){
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(sp->fd, &rfds);
    struct timeval tv;
    tv.tv_sec  = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    int rv = select(sp->fd + 1, &rfds, NULL, NULL, &tv);
    if (rv < 0)  return -1;          // erro select
    if (rv == 0) return 0;           // timeout (sem dados)
    int r = (int)read(sp->fd, buf, maxlen);
    return (r >= 0) ? r : -1;
}
