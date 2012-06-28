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
#include <vector>
#include <utilities.h>
#include <fs.h>
#ifdef PLATFORM_WIN32
#include <fs/directory/directory_defines-win32.h>
#elif defined(PLATFORM_POSIX)
#include <fs/directory/directory_defines-posix.h>
#endif
namespace os {namespace fs {

#ifdef PLATFORM_WIN32
void ChangeDirectory(const char* path) {
  SetCurrentDirectory(path);
}

void Chmod(const char* path, int permiss) {
  _chmod(path,permiss);
}
#elif defined(PLATFORM_POSIX)
void ChangeDirectory(const char* path) {
  ::chdir(path);
}

void Chmod(const char* path, int permiss) {
  ::chmod(path, permiss);
}
#endif

void ChangeDirectory(const std::string& path) {
  ChangeDirectory(path.c_str());
}

void Chmod(const std::string& path, int permiss) {
  Chmod(path.c_str(), permiss);
}

DirectoryIterator::DirectoryIterator(const char* path, bool recursive)
    : recursive_(recursive),
      dir_data_(NULL),
      current_entry_(NULL) {Initialize(path);}

DirectoryIterator::DirectoryIterator(const std::string& path, bool recursive)
    : recursive_(recursive),
      dir_data_(NULL),
      current_entry_(NULL) {Initialize(path.c_str());}

DirectoryIterator::DirectoryIterator(const Path& path_info, bool recursive)
    : recursive_(recursive),
      dir_data_(NULL),
      current_entry_(NULL) {
  if (path_info.HasAbsolutePath()) {
    Initialize(path_info.absolute_path());
  }
}

DirectoryIterator::DirectoryIterator()
    : recursive_(false),
      dir_data_(NULL),
      current_entry_(NULL) {}

DirectoryIterator::DirectoryIterator(const DirectoryIterator& iterator)
    : recursive_(iterator.recursive_) {
  if (dir_data_ != NULL) {
    delete dir_data_;
  }
  dir_data_ = iterator.dir_data_;
  if (current_entry_ != NULL) {
    delete current_entry_;
  }
  current_entry_ = iterator.current_entry_;
  sub_ = iterator.sub_;
}

DirectoryIterator::~DirectoryIterator() {
  if (dir_data_ != NULL) {
    Dir::Close(dir_data_);
    delete dir_data_;
  }
  if (current_entry_ != NULL) {
    delete current_entry_;
  }
}

void DirectoryIterator::Initialize(const char* path) {
  if (dir_data_ == NULL) {
    dir_data_ = new DirData;
  } else {
    Dir::Close(dir_data_);
  }
  ReadDirectory(Dir::Open(path, dir_data_));
}

const DirectoryIterator& DirectoryIterator::operator = (const DirectoryIterator& iterator) {
  recursive_ = iterator.recursive_;
  sub_ = iterator.sub_;
  dir_data_ = iterator.dir_data_;
  if (current_entry_ != NULL) {
    delete current_entry_;
  }
  current_entry_ = iterator.current_entry_;
  return (*this);
}

const DirEntry* DirectoryIterator::operator*() const {
  return current_entry_;
}

const DirEntry* DirectoryIterator::operator->() const {
  return current_entry_;
}

DirectoryIterator& DirectoryIterator::operator++() {
  if (dir_data_) {
    ReadDirectory(Dir::NextEntry(dir_data_));
  }
  return (*this);
}

bool DirectoryIterator::operator != (const DirectoryIterator& iter) const {
  return current_entry_ != iter.current_entry_;
}

void DirectoryIterator::CreateDirEnt() {
  if (dir_data_ != NULL) {
    current_entry_ = (current_entry_ != NULL)?
        Dir::CreateNextEntry(dir_data_, current_entry_) : Dir::CreateFirstEntry(dir_data_);
  }
}


void DirectoryIterator::ReadDirectory(bool success) {
  if (!success) {
    if (current_entry_ != NULL) {
      delete current_entry_;
      current_entry_ = NULL;
    }
  } else {
    CreateDirEnt();
    if (current_entry_ != NULL && current_entry_->IsDir() && recursive_) {
      sub_.push_back(std::string(current_entry_->abspath()));
    }
    if (current_entry_ != NULL && filter_base_) {
      if (!filter_base_(current_entry_)) {operator++();}
    }
  }
  if (current_entry_ == NULL && recursive_ && sub_.size() > 0) {
    std::string abspath = sub_.back();
    sub_.pop_back();
    Initialize(abspath.c_str());
  } else if (current_entry_ == NULL) {
    Dir::Close(dir_data_);
    delete dir_data_;
    dir_data_ = NULL;
  }
}
static DirectoryIterator it;
const DirectoryIterator& DirectoryIterator::end() {return it;}
}}
