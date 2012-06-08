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
#ifndef FS_DIRECTORY_DIRECTORY_DEFINES_POSIX_H_
#define FS_DIRECTORY_DIRECTORY_DEFINES_POSIX_H_
#include <string.h>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <utilities.h>
#include <fs.h>

namespace os {namespace fs {
struct DirData {
  std::string dirpath;
  dirent entry;
  dirent* result;
  DIR* dir;
};

class Dir {
 public :
  static bool Open(const char* path, DirData* data);
  static void Close(DirData* data);
  static bool NextEntry(DirData* data);
  static DirEntry* CreateFirstEntry(DirData* data, memory::Pool* pool);
  static DirEntry* CreateNextEntry(DirData* data, memory::Pool* pool);
};

}}

#endif

