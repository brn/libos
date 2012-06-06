#ifndef FS_EVENT_MACOS_FSEVENT_FS_WATCHER_H_
#define FS_EVENT_MACOS_FSEVENT_FS_WATCHER_H_
#include <fs/event/event.h>
#ifdef PLATFORM_MACOS
#include <fs/event/macos/fs_watcher_macos.h>
#elif defined PLATFORM_WIN32
#include <fs/event/win32/fs_watcher_win32.h>
#elif defined PLATFORM_POSIX
#include <fs/event/linux/fs_watcher_linux.h>
#endif
#endif
