#ifndef FS_DIRECTORY_DIRECTORY_INL_H_
#define FS_DIRECTORY_DIRECTORY_INL_H_
#include <fs.h>
namespace os {namespace fs {
/*
Directory::const_iterator::const_iterator(const DirEntry* entry) : entry_(entry){}
Directory::const_iterator::const_iterator() : entry_(0){}
Directory::const_iterator::~const_iterator() {}

Directory::const_iterator::const_iterator(const Directory::const_iterator& iterator) {
  entry_ = iterator.entry_;
}

const Directory::const_iterator& Directory::const_iterator::operator = (const Directory::const_iterator& iterator) {
  entry_ = iterator.entry_;
  return (*this);
}

const DirEntry& Directory::const_iterator::operator*() const {
  return (*entry_);
}

const DirEntry* Directory::const_iterator::operator->() const {
  return entry_;
}

Directory::const_iterator& Directory::const_iterator::operator++() {
  entry_ = entry_->next_;
  return (*this);
}

bool Directory::directory_iterator::operator != (const Directory::const_iterator& iter) const {
  return entry_ != iter.entry_;
}
*/

}}
#endif
