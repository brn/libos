#ifndef OS_INCLUDE_FS_STAT_H_
#define OS_INCLUDE_FS_STAT_H_
#include <sys/stat.h>
#include <sys/types.h>
namespace os { namespace fs {
class Stat{
 public :
  typedef enum {
    kFifo,
    kChr,
    kDir,
    kBlk,
    kReg,
    kLnk,
    kSock
  } FileType;
  Stat(const char* path);
  ~Stat();
  bool IsExist() const { return is_exist_; }
  int Dev() const { return fstat_.st_dev;}
  int Ino() const { return fstat_.st_ino; }
  int NLink() const { return fstat_.st_nlink; }
  int UId() const { return fstat_.st_uid; }
  int GId() const { return fstat_.st_gid; };
  int RDev() const { return fstat_.st_rdev; };
  int Size() const { return fstat_.st_size; };
  const char* ATime();
  const char* MTime();
  const char* CTime();
  bool IsDir();
  bool IsReg();
  bool IsChr();
  bool IsLink();
  static bool IsExist(const char* path);
  static bool IsLink(const char* path);
  static bool IsDir(const char* path);
  static bool IsReg(const char* path);
 private :
  bool is_exist_;
  char* path_;
  char atime_[200];
  char mtime_[200];
  char ctime_[200];
#ifdef PLATFORM_WIN32
  struct _stat fstat_;
#elif defined PLATFORM_POSIX
  struct stat fstat_;
#endif
};
}}
#endif
