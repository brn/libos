#ifndef FS_EVENT_MACOS_FS_WATCHER_H_ 
#define FS_EVENT_MACOS_FS_WATCHER_H_ 
#include "utilities.h"
#include "smart_pointer/scoped_ptr.h"
#include "smart_pointer/shared_ptr.h"
#include "notificator/notificator.h"
#include "lib/unordered_map.h"
namespace os {namespace fs {
class FSEvent;
class FSEventData;
class FSWatcher : public Notificator<FSEvent*>{
  typedef SharedPtr<FSEventData> FSEventDataHandle;
  typedef SharedPtr<FSEvent> FSEventHandle;
  typedef std::pair<const char*, FSEventDataHandle> FSEventPair;
  typedef roastlib::unordered_map<std::string, FSEventDataHandle> FSEventMap;
 public :
  FSWatcher();
  ~FSWatcher();
  bool AddWatch (const char* path);
  bool RemoveWatch(const char* path);
  void RemoveWatch();
  bool IsWatched(const char* path);
  bool Run();
  bool RunAsync();
  void Exit();
  bool IsRunning() const;
  static const char kModify[];
  static const char kUpdate[];
  static const char kDelete[];
 private :
  void Start();
  int GetState();
  static void* ThreadRunner(void* param);
  BitVector8 flags_;
  int kq_;
  Mutex mutex_;
  FSEventMap map_;
};

}}
#endif
