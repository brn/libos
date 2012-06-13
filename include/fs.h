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
bool Move(const char* old_path, const char* new_path);
bool Remove(const char* path);
void ChangeDirectory(const char* path);
void Chmod(const char* path, int permiss);
bool CreateDirectory(const char* path, int permiss);
class FSWatcher;
class FSEvent {
  friend class FSWatcher;
 public :
  ~FSEvent();
  bool IsExist() const;
  bool IsModified();
  bool IsUpdate();
  void Close();
  FSWatcher* watcher() {return fs_watcher_;}
  const char* filename() const;
  const char* md5() const;
 private :
  FSEvent(const char* path, FSWatcher*);
  std::string path_;
  std::string mtime_;
  std::string md5_;
  FSWatcher* fs_watcher_;
};
}}

#include "fs_include/fs_path.h"
#include "fs_include/fs_stat.h"
#include "fs_include/fs_directory.h"
#include "fs_include/fs_io.h"

#ifdef PLATFORM_MACOS
#include "fs_include/fs_watcher_macos.h"
#elif defined PLATFORM_WIN32
#include "fs_include/fs_watcher_win32.h"
#elif defined PLATFORM_POSIX
#include "fs_include/fs_watcher_linux.h"
#endif
#endif


