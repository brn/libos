#ifndef FS_EVENT_LINUX_FS_WATCHER_H_
#define FS_EVENT_LINUX_FS_WATCHER_H_
#include <sys/inotify.h>
#include <vector>
#include <lib/unordered_map.h>
#include "notificator/notificator.h"
#include "smart_pointer/shared_ptr.h"
namespace os { namespace fs {
class FSEvent;
typedef SharedPtr<FSEvent> FSEventHandle;
typedef std::pair<int, FSEventHandle> InotifyFDPair;
typedef unordered_map<int,  FSEventHandle> InotifyFDMap;
typedef unsigned long InotifyMask;
typedef SharedPtr<inotify_event> EventHandle;
typedef struct inotify_event InotifyEvent;
class FSWatcher : public Notificator<FSEvent*>{
 public :
  FSWatcher();
  ~FSWatcher();
  void AddWatch(const char* path);
  void RemoveWatch(const char* path);
  void RemoveWatch();
  void Run();
  void RunAsync();
  void Exit();
  bool IsRunning() const {return !is_exit_;};
  bool IsWatched(const char* path) const;
  static const char kModify[];
  static const char kUpdate[];
  static const char kDelete[];
 private :
  static void* ThreadRunner(void* args);
  void Regist(const char* abpath);
  void Start();
  void ReadInotifyEvents();
  void CheckEvent(InotifyEvent* ev);
  bool is_exit_;
  int epoll_fd_;
  int inotify_fd_;
  Mutex mutex_;
  InotifyFDMap fd_map_;
};
}}

#endif
