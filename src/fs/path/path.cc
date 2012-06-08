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
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#ifdef PLATFORM_WIN32
#include <windows.h>
#elif defined PLATFORM_POSIX
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#endif
#include <fs.h>
#include <thread.h>
#include <utilities.h>

#ifdef PLATFORM_WIN32
#define HOME "HOMEPATH"
#elif defined PLATFORM_POSIX
#define HOME "HOME"
#endif

#ifndef MAXPATHLEN
#define MAXPATHLEN 10000
#else
#undef MAXPATHLEN
#define MAXPATHLEN 10000
#endif

#ifdef PLATFORM_POSIX
#define FULL_PATH(path, tmp) tmp = realpath(path, NULL)
#elif defined PLATFORM_WIN32
#define FULL_PATH(path, tmp) tmp = _fullpath(NULL, path, 0)
#endif

namespace os {namespace fs {
static os::Mutex mutex_;
char* GetDirectoryFromPath(const char* path, char* buffer) {
  int index = strlen(path);
  bool is_slashed = false;
  while (index--) {
    if (is_slashed) {
      break;
    }
    if (path[index] == '/') {
      is_slashed = true;
    }
  }
  buffer = (is_slashed)? os::Strdup(&path[index + 1]) : os::Strdup(path);
  return buffer;
}

char* GetFileNameFromPath(const char* path, char* buffer) {
  int len = strlen(path);
  if (path[len - 1] == '/') {
    return NULL;
  }
  const char* ptr = strrchr(path, '/');
  if (ptr) {
    buffer = os::Strdup(ptr + 1);
  } else {
    buffer = os::Strdup(path);
  }
  return buffer;
}

char* ConvertBackSlash(const char* path, char* buffer) {
  buffer = os::Strdup(path);
  for (int i = 0; buffer[i]; i++) {
    if (buffer[i] == '\\') {
      buffer[i] = '/';
    }
  }
  return buffer;
}

char* GetExtension(const char* path, char* buffer) {
  bool has = false;
  int begin = strlen(path) - 1;
  for (int i = begin; path[i]; i--) {
    if (path[i] == '.' && i < begin) {
      has = true;
      buffer = os::Strdup(&path[i + 1]);
      break;
    }
  }
  if (!has) {
    return NULL;
  }
  return buffer;
}

char* GetAbsolutePath(const char* path, char* buffer, bool* is_success = 0) {
  if (strcmp(path, "/") == 0) {
    buffer = os::Strdup(path);
    return NULL;
  }
  char *tmp;
  FULL_PATH(path, tmp);
  if (tmp != NULL) {
    buffer = ConvertBackSlash(tmp, buffer);
    free(tmp);
  } else {
    if (is_success) {
      (*is_success) = false;
    }
    buffer = os::Strdup(path);
  }
  return buffer;
}

void Path::NormalizePath(const char* path, std::string* buffer) {
  int size = strlen(path);
  char* buf;
  buf = ConvertBackSlash(path, buf);
  buffer->assign(buf);
  free(buf);
  while (1) {
    size_t pos = buffer->find("../", 0);
    if (pos == std::string::npos) {
      size_t pos = buffer->find("./", 0);
      if (pos != std::string::npos) {
        buffer->erase(pos, 2);
      } else {
        size_t pos = buffer->find("//", 0);
        if (pos != std::string::npos) {
          buffer->erase(pos, 1);
        } else {
          break;
        }
      }
    } else if (pos){
      int count = 0;
      int matched = 0;
      int spos = pos;
      int ssize = size;
      bool has_ch = false;
      while (spos < ssize && spos > -1) {
        if (buffer->at(spos) == '/') {
          if (matched == 1 && has_ch) {
            break;
          }
          matched = 1;
        }
        if (buffer->at(spos) != '.' && buffer->at(spos) != '/') {
          has_ch = true;
        }
        spos--;
        count++;
      }
      if (spos < 0) {
        spos = 0;
      }
      buffer->erase(spos, count + 2);
    }
  }
  if (buffer->at(buffer->size() - 1) == '/') {
    buffer->erase(buffer->size() - 1, buffer->size());
  }
}

Path::Path(const char* path)
    : raw_(NULL),
      fullpath_(NULL),
      filename_(NULL),
      directory_(NULL),
      ext_(NULL){
  raw_ = os::Strdup(path);
  bool success = true;
  fullpath_ = GetAbsolutePath(path, fullpath_, &success);
  if (fullpath_ != NULL) {
    int len = strlen(fullpath_);
    if (len > 1 && fullpath_[len - 1] == '/') {
      fullpath_[len - 1] = 0;
    }
    if (success) {
      Stat stat(fullpath_);
      if (!stat.IsDir()) {
        directory_ = GetDirectoryFromPath(absolute_path(), directory_);
        filename_ = GetFileNameFromPath(absolute_path(), filename_);
        ext_ = GetExtension(absolute_path(), ext_);
      } else {
        directory_ = os::Strdup(fullpath_);
        filename_ = os::Strdup("");
        ext_ = NULL;
      }
    } else {
      directory_ = os::Strdup(path);
      filename_ = os::Strdup(path);
    }
  }
}

Path::~Path() {
  if (raw_ != NULL) {free(raw_);}
  if (fullpath_ != NULL) {free(fullpath_);}
  if (filename_ != NULL) {free(filename_);}
  if (directory_ != NULL) {free(directory_);}
}

const char* Path::current_directory() {
  os::ScopedLock lock(mutex_);
  if (current_dir_ != NULL) {
    free(current_dir_);
  } else {
    atexit(Path::ReleaseCurrentDirectory);
  }
  current_dir_ = NULL;
#define GW_BUF_SIZE 1000
#ifdef PLATFORM_WIN32
    char tmp[GW_BUF_SIZE];
    DWORD isSuccess = GetCurrentDirectory(sizeof(tmp), tmp);
    if (!isSuccess) {
      fprintf(stderr, "GetCwd fail.");
    }
    ConvertBackSlash(tmp, current_dir_);
#elif defined PLATFORM_POSIX
    char tmp[GW_BUF_SIZE];
    char* dir = getcwd(tmp, sizeof (tmp));
    if (!dir) {
      fprintf(stderr, "GetCwd fail.");
    };
    current_dir_ = os::Strdup(dir);
#endif
    return current_dir_;
}


const char* Path::home_directory() {
  os::ScopedLock lock(mutex_);
  if (user_home_ != NULL) {
    free(user_home_);
  } else {
    atexit(Path::ReleaseHomeDirectory);
  }
  user_home_ = NULL;
#ifdef PLATFORM_WIN32
  const char* drive = getenv("HOMEDRIVE");
  const char* home = getenv(HOME);
  if (home && drive) {
    std::string buf;
    os::SPrintf(&buf, "%s/%s", drive, home);
    user_home_ = os::Strdup(buf.c_str());
    GetAbsolutePath(user_home_, user_home_);
    return user_home_;
  }
  return 0;
#elif defined PLATFORM_POSIX
  const char* home = getenv(HOME);
  if (home) {
    user_home_ = os::Strdup(home);
    return user_home_;
  }
  return 0;
#endif
}

void Path::ReleaseCurrentDirectory() {
  if (current_dir_ != NULL) {
    free(current_dir_);
  }
}
void Path::ReleaseHomeDirectory() {
  if (user_home_ != NULL) {
    free(user_home_);
  }
}

char* Path::current_dir_ = NULL;
char* Path::user_home_ = NULL;

typedef std::vector<std::string> PathArray;

void GetPathArray(const char* path, PathArray *array) {
  int len = strlen(path);
  std::string tmp = path;
  std::string slash = "/";
  std::string str;
  if (path[ len - 1 ] != '/') {
    tmp += '/';
  }
  const char* raw = tmp.c_str();
  int i = 0;
  while (raw[ i ]) {
    if (raw[ i ] == '/') {
      if (i == 0) {
        array->push_back(slash);
      } else {
        array->push_back(str);
      }
      str.clear();
    } else {
      str += raw[i];
    }
    i++;
  }
}

const char* fs::Path::relative_path(const char* base, const char* path, std::string* buf) {
  if (strcmp(base, path) == 0) {
    return base;
  }
  PathArray base_array;
  PathArray target_array;
  GetPathArray(base, &base_array);
  GetPathArray(path, &target_array);
  int i = 0;
  int base_size = base_array.size();
  int target_size = target_array.size();
  while ((i < base_size) || (i < target_size)) {
    if (i >= base_size) {
      buf->append(target_array.at(i));
      buf->append("/");
    } else if (i >= target_size) {
      std::stringstream st;
      while (i < base_size) {
        st << "../";
        i++;
      }
      buf->append(st.str());
    } else if (base_array.at(i).compare(target_array.at(i)) != 0) {
      std::stringstream st;
      while (i < base_size) {
        st << "../";
        base_array.pop_back();
        base_size = base_array.size();
      }
      while (i < target_size) {
        st << target_array[ i ];
        st << "/";
        i++;
      }
      buf->append(st.str());
    }
    i++;
  }
  if (buf->size() > 1 && buf->at(buf->size() - 1) == '/') {
    buf->erase(buf->size() - 1, 1);
  }
  return buf->c_str();
}


}}

