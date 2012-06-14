#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <defines.h>
#ifdef PLATFORM_WIN32
#include <direct.h>
#define MKDIR(path) ::_mkdir(path)
#elif defined PLATFORM_POSIX
#include <sys/stat.h>
#define MKDIR(path) ::mkdir(path,0777)
#endif
#include <fs.h>
#include <thread.h>
#include <utilities.h>
namespace os {namespace fs {
class MutexHolder : private Static {
 public :
  static os::Mutex mutex;
};

os::Mutex MutexHolder::mutex;

bool MakeDirectory(const char* path, int permiss) {
  os::ScopedLock lock(MutexHolder::mutex);
  int len = strlen(path);
  if (len > 0) {
    std::string processed_path = path;
    if (path[ len - 1 ] != '/') {
      processed_path += '/';
      len += 1;
    }
    const char* current = os::fs::Path::current_directory();
    char tmp[ 200 ];
    for (int i = 0,count = 0; i < len; ++i) {
      if (processed_path[ i ] == '/') {
        if (i == 0) {
          chdir("/");
        } else {
          if (tmp[ count - 1 ] == ':') {
            tmp[ count ] = '/';
            count++;
          } 
          tmp[ count ] = '\0';
          Stat st(tmp);
          if (!st.IsExist() || !st.IsDir()) {
            if (-1 == MKDIR(tmp)) {
              ChangeDirectory(current);
              return false;
            }
            Chmod(tmp, permiss);
            ChangeDirectory(tmp);
          } else if (st.IsDir()) {
            ChangeDirectory(tmp);
          }
          tmp[ 0 ] = '\0';
          count = 0;
        }
      } else {
        tmp[ count ] = processed_path[ i ];
        count++;
      }
    }
    ChangeDirectory(current);
    return true;
  }
  return false;
}

bool MakeDirectory(const std::string& path, int permiss) {
  return MakeDirectory(path.c_str(), permiss);
}
}}
