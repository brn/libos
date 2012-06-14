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

directory_iterator::directory_iterator(const char* path, bool recursive)
    : recursive_(recursive),
      dir_data_(NULL),
      current_entry_(NULL),
      filter_base_(NULL){Initialize(path);}

directory_iterator::directory_iterator(const std::string& path, bool recursive)
    : recursive_(recursive),
      dir_data_(NULL),
      current_entry_(NULL),
      filter_base_(NULL){Initialize(path.c_str());}

directory_iterator::directory_iterator(const Path& path_info, bool recursive)
    : recursive_(recursive),
      dir_data_(NULL),
      current_entry_(NULL),
      filter_base_(NULL){
  if (path_info.HasAbsolutePath()) {
    Initialize(path_info.absolute_path());
  }
}

directory_iterator::directory_iterator()
    : recursive_(false),
      dir_data_(NULL),
      current_entry_(NULL),
      filter_base_(NULL){}

directory_iterator::directory_iterator(const directory_iterator& iterator)
    : recursive_(iterator.recursive_),
      dir_data_(iterator.dir_data_) {
  if (current_entry_ != NULL) {
    delete current_entry_;
  }
  current_entry_ = iterator.current_entry_;
}

directory_iterator::~directory_iterator() {
  if (dir_data_ != NULL) {
    Dir::Close(dir_data_);
    delete dir_data_;
  }
  if (current_entry_ != NULL) {
    delete current_entry_;
  }
  if (filter_base_ != NULL) {
    delete filter_base_;
  }
}

void directory_iterator::Initialize(const char* path) {
  if (dir_data_ == NULL) {
    dir_data_ = new DirData;
  } else {
    Dir::Close(dir_data_);
  }
  ReadDirectory(Dir::Open(path, dir_data_));
}

const directory_iterator& directory_iterator::operator = (const directory_iterator& iterator) {
  recursive_ = iterator.recursive_;
  sub_ = iterator.sub_;
  dir_data_ = iterator.dir_data_;
  if (current_entry_ != NULL) {
    delete current_entry_;
  }
  current_entry_ = iterator.current_entry_;
  return (*this);
}

const DirEntry* directory_iterator::operator*() const {
  return current_entry_;
}

const DirEntry* directory_iterator::operator->() const {
  return current_entry_;
}

directory_iterator& directory_iterator::operator++() {
  if (dir_data_) {
    ReadDirectory(Dir::NextEntry(dir_data_));
  }
  return (*this);
}

bool directory_iterator::operator != (const directory_iterator& iter) const {
  return current_entry_ != iter.current_entry_;
}

void directory_iterator::CreateDirEnt() {
  if (dir_data_ != NULL) {
    current_entry_ = (current_entry_ != NULL)?
        Dir::CreateNextEntry(dir_data_, current_entry_) : Dir::CreateFirstEntry(dir_data_);
  }
}


void directory_iterator::ReadDirectory(bool success) {
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
    if (current_entry_ != NULL && filter_base_ != NULL) {
      if (!filter_base_->Invoke(current_entry_)) {operator++();}
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
static directory_iterator it;
const directory_iterator& directory_iterator::end() {return it;}
}}
