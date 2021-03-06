#ifndef FS_EVENT_MACOS_FS_WATCHER_H_ 
#define FS_EVENT_MACOS_FS_WATCHER_H_ 
#include "../utilities.h"
#include "../thread.h"
#include "../notificator/notificator.h"
#include "../lib/unordered_map.h"
#include "../lib/shared_ptr.h"
namespace os {namespace fs {
class FSEvent;
class FSEventData;
class FSWatcher : public Notificator<void (FSEvent*)>{
  typedef shared_ptr<FSEventData> FSEventDataHandle;
  typedef shared_ptr<FSEvent> FSEventHandle;
  typedef std::pair<const char*, FSEventDataHandle> FSEventPair;
  typedef unordered_map<std::string, FSEventDataHandle> FSEventMap;
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
  BitVector8 flags_;
  int kq_;
  mutex mutex_;
  FSEventMap map_;
};

}}
#endif
