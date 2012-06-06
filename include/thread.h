#ifndef INCLUDE_THREAD_H_
#define INCLUDE_THREAD_H_
#include "defines.h"
#ifdef PLATFORM_POSIX
#include "thread-posix.h"
#elif defined PLATFORM_WIN32
#include "thread-win32.h"
#endif

#endif
