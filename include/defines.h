#if defined(__APPLE__) && defined(__MACH__)
#define PLATFORM_MACOS
#define PLATFORM_POSIX
#elif defined __GNUC__ && defined(__linux__)
#define PLATFORM_POSIX
#elif defined _WIN32
#define PLATFORM_WIN32
#endif

