#ifdef PLATFORM_MACOS
#include "fs_watcher_macos.h"
#elif defined PLATFORM_WIN32
#include "fs_watcher_win32.h"
#elif defined PLATFORM_POSIX
#include "fs_watcher_linux.h"
#endif
