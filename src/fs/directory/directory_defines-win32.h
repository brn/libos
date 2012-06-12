#ifndef FS_DIRECTORY_DIRECTORY_DEFINES_WIN32_H_
#define FS_DIRECTORY_DIRECTORY_DEFINES_WIN32_H_
#include <windows.h>
#include <io.h>
#include <utilities.h>
#include <fs.h>
namespace os { namespace fs {
struct DirData {
  std::string dirpath;
  WIN32_FIND_DATA ffdata;
  HANDLE h_find;
};

class Dir {
 public :
  static bool Open(const char* path, DirData* data);
  static void Close(DirData* data);
  static bool NextEntry(DirData* data);
  static DirEntry* CreateFirstEntry(DirData* data);
  static DirEntry* CreateNextEntry(DirData* data, DirEntry* entry);
};
  
}}

#endif
