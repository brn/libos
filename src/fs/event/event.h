#ifndef FS_EVENT_EVENT_H_
#define FS_EVENT_EVENT_H_
#include <string>
namespace os {namespace fs {
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
#endif
