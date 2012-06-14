#include <time.h>
#include <fs.h>

#ifdef PLATFORM_POSIX
#define STAT_FN(filename, statObj) ::lstat (filename, statObj)
#elif defined(PLATFORM_WIN32)
#define STAT_FN(filename, statObj) ::_stat (filename, statObj)
#endif

#ifdef PLATFORM_WIN32
#define CTIME(str,buf) ::_ctime64_s(buf,200,str)
#elif defined PLATFORM_POSIX
#define CTIME(str,buf) ::ctime_r(str,buf)
#endif

#define MODE (fstat_.st_mode & S_IFMT)

namespace os {namespace fs {
Stat::Stat(const char* path) {
  path_ = Strdup(path);
  is_exist_ = (STAT_FN(path, &fstat_) != -1);
}

Stat::~Stat() {free(path_);}

const char* Stat::ATime() {
  CTIME(&(fstat_.st_atime),atime_);
  return atime_;
}
const char* Stat::MTime() {
  CTIME(&(fstat_.st_mtime),mtime_);
  return mtime_;
}
const char* Stat::CTime() {
  CTIME(&(fstat_.st_ctime),ctime_);
  return ctime_;
}
bool Stat::IsDir() { return MODE == S_IFDIR; }
bool Stat::IsReg() { return MODE == S_IFREG; }
bool Stat::IsChr() { return MODE == S_IFCHR; }
bool Stat::IsLink() {
#ifdef PLATFORM_WIN32
  Path path_info(path_);
  if (path_info.HasExtension()) {
    const char* ext = path_info.extension();
    return strcmp(ext, "lnk") == 0;
  }
  return false;
#else
  return MODE == S_IFLNK;
#endif
}

bool Stat::IsLink(const char* path) {
  Stat stat(path);
  return stat.IsLink();
}

bool Stat::IsDir(const char* path) {
  Stat stat(path);
  return stat.IsDir();
}

bool Stat::IsReg(const char* path) {
  Stat stat(path);
  return stat.IsReg();
}

bool Stat::IsExist(const char* path) {
  Stat stat(path);
  return stat.IsExist();
}
}}
