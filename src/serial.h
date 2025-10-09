#ifndef SERIAL_ABS_H
#define SERIAL_ABS_H

#ifdef _WIN32
  #include "serial_win32.h"
#else
  #include "serial_posix.h"
#endif

#endif
