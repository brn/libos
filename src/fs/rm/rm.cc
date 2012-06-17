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
#include <stdio.h>
#include <defines.h>
#ifdef PLATFORM_POSIX
#include <unistd.h>
#define RMDIR(path) ::rmdir(path)
#elif defined(PLATFORM_WIN32)
#include <direct.h>
#define RMDIR(path) ::_rmdir(path)
#endif
#include <fs.h>
namespace os { namespace fs {

bool RemoveDirOnly(const char* path) {
  DirectoryIterator dirs(path, true);
  DirectoryIterator end;
  while (dirs != end) {
    RemoveDirOnly(dirs->abspath());
    ++dirs;
  }
  return RMDIR(path) != -1;
}

bool RemoveDir(const char* path) {
  DirectoryIterator dirs(path, true);
  DirectoryIterator end;
  while (dirs != end) {
    if (dirs->IsFile()) {
      bool ret = (::remove(dirs->abspath()) != -1);
      if (!ret) {
        return ret;
      }
    }
    ++dirs;
  }  
  return RemoveDirOnly(path);
}

bool Remove(const char* path) {
  Stat stat(path);
  if (stat.IsExist()) {
    if (stat.IsDir()) {
      return RemoveDir(path);
    }
    return (::remove(path) != -1);
  } else {
    return false;
  }
}

bool Remove(const std::string& path) {return Remove(path.c_str());}
}}
