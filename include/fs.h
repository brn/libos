/**
 *@author Taketoshi Aono
 *@fileOverview
 *@license
 *Copyright (c) 2011 Taketoshi Aono
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 *associated doc umentation files (the "Software"), to deal in the Software without restriction,
 *including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 *subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all copies or
 *substantial portions ofthe Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 *TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 *CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *DEALINGS IN THE SOFTWARE.
 */
#ifndef INCLUDE_FS_H_
#define INCLUDE_FS_H_
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <vector>
#include "defines.h"
#include "thread.h"
#include "utilities.h"
#include "notificator/listener_adapter.h"
namespace os { namespace fs {
class Path {
 public :
  Path(const char* path);
  ~Path();
  const char* raw_path() const { return raw_; }
  const char* absolute_path() const { return fullpath_; }
  const char* filename() const { return filename_; }
  const char* directory() const { return directory_; }
  const char* extension() const {return ext_;}
  bool HasFilename() const { return filename_ != NULL; }
  bool HasDirectory() const { return directory_ != NULL; }
  bool HasAbsolutePath() const { return fullpath_ != NULL; }
  bool HasExtension() const { return ext_ != NULL; }
  static const char* current_directory();
  static const char* home_directory();
  static const char* relative_path(const char* base, const char* dest, std::string* buf);
  static void NormalizePath(const char* path, std::string* buf);
 private :
  static void ReleaseCurrentDirectory();
  static void ReleaseHomeDirectory();
  char* raw_;
  char* fullpath_;
  char* filename_;
  char* directory_;
  char* ext_;
  static char* current_dir_;
  static char* user_home_;
};

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
  ~Stat(){};
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
 private :
  bool is_exist_;
  const char* path_;
  char atime_[200];
  char mtime_[200];
  char ctime_[200];
#ifdef PLATFORM_WIN32
  struct _stat fstat_;
#elif defined PLATFORM_POSIX
  struct stat fstat_;
#endif
};

bool Move(const char* old_path, const char* new_path);
bool Remove(const char* path);
class DirFinder;
struct DirData;
class DirEntry : public memory::Allocated {
  friend class Directory;
  friend class DirFinder;
 public :
  DirEntry(const char* path, const char* dir, const char* fullpath, bool is_dir)
      : is_dir_(is_dir) {
    name_ = os::Strdup(path);
    dir_ = os::Strdup(dir);
    full_path_ = os::Strdup(fullpath);
  }
  ~DirEntry(){};
  const char* name() const { return name_; };
  const char* abspath() const { return full_path_; };
  const char* dirname() const { return dir_; };
  bool IsDir() const {return is_dir_;}
  bool IsFile() const {return !is_dir_;}  
 private :
  DirEntry(){}
  bool is_dir_;
  char* name_;
  char* dir_;
  char* full_path_;
};

class directory_iterator : public std::iterator<std::forward_iterator_tag, const DirEntry*> {
 public :
  directory_iterator(const char* path, bool recursive = false);
  directory_iterator(const std::string& path, bool recursive = false);
  directory_iterator(const Path& path_info, bool recursive = false);
  template <typename FilterT>
  directory_iterator(const char* path, FilterT f, bool recursive = false)
      : recursive_(recursive),
        dir_data_(NULL),
        current_entry_(NULL),
        filter_base_(new Filter<FilterT>(f)) {Initialize(path);}
  
  directory_iterator();
  directory_iterator(const directory_iterator&);
  ~directory_iterator();
  const directory_iterator& operator = (const directory_iterator&);
  const DirEntry* operator*() const;
  const DirEntry* operator->() const;
  directory_iterator& operator ++();
  bool operator !=(const directory_iterator&) const;
  static const directory_iterator& end();
 private :
  class FilterBase {
   public :
    virtual bool Invoke(DirEntry* ent) = 0;
  };
  template <typename T>
  class Filter : public FilterBase {
   public :
    Filter(T fn)
    : fn_(fn) {}
    virtual bool Invoke(DirEntry* ent) {return Dereferrence<T>::Get(fn_)(ent);}
   private :
    T fn_;
  };
  void Initialize(const char* path);
  void CreateDirEnt();
  void ReadDirectory(bool);
  bool recursive_;
  std::vector<DirEntry*> sub_;
  DirData* dir_data_;
  DirEntry* current_entry_;
  memory::Pool pool_;
  FilterBase* filter_base_;
};

void ChangeDirectory(const char* path);
void Chmod(const char* path, int permiss);

bool CreateDirectory(const char* path, int permiss);

class FSWatcher;
class FSEvent {
 public :
  FSEvent(const char* path, FSWatcher*);
  ~FSEvent();
  bool IsExist() const;
  bool IsModified();
  bool IsUpdate();
  void Close();
  FSWatcher* watcher() {return fs_watcher_;}
  const char* filename() const;
  const char* md5() const;
 private :
  std::string path_;
  std::string mtime_;
  std::string md5_;
  FSWatcher* fs_watcher_;
};
}}

#ifdef PLATFORM_MACOS
#include "fs_watcher_macos.h"
#elif defined PLATFORM_WIN32
#include "fs_watcher_win32.h"
#elif defined PLATFORM_POSIX
#include "fs_watcher_linux.h"
#endif
#endif

