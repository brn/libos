#define DEFAULE_WORKER_MAX 100
#include <fcntl.h>
#include <stdio.h>
#include <gtest/gtest.h>
#include <string.h>
#include "../include/thread.h"
#include "../include/lib/unordered_map.h"
#include "../include/lib/atomic.h"
#include "../include/utilities.h"
#include "../include/fs.h"
using namespace os::fs;
namespace os {
typedef os::unordered_map<std::string, int> UTTestFiles;
typedef os::unordered_map<std::string, int> UTTestStamp;
static UTTestFiles files;
static UTTestStamp update_stamp;
static UTTestStamp modify_stamp;
static int count = 0;
static mutex mt;
static int all = 0;
void Init(int begin = 0, int end = 100) {
  for (; begin < end; begin ++) {
    std::string buf;
    os::SPrintf(&buf, CURRENT_DIR"/test/ut_watched_tmp_%d", begin);
    FILE* fp = os::FOpen(buf.c_str(), "wb");
    DEBUG_LOG(Info, "%s", buf.c_str());
    if (fp != NULL) {
      os::FClose(fp);
      files[buf.c_str()] = 1;
      all++;
    }
  }
}

void CleanUp() {
  UTTestFiles::iterator it = files.begin();
  for (; it != files.end(); ++it) {
    os::fs::Remove(it->first.c_str());
  }
}

void Watch(FSWatcher* watcher) {
  UTTestFiles::iterator it = files.begin();
  for (; it != files.end(); ++it) {
    if (!watcher->IsWatched(it->first.c_str())) {
      watcher->AddWatch(it->first.c_str());
    }
  }
}

void WatchMulti(FSWatcher* watcher, FSWatcher* watcher2) {
  int fcount = 0;
  UTTestFiles::iterator it = files.begin();
  for (; it != files.end(); ++it) {
      if (fcount % 2 == 0) {
        if (!watcher->IsWatched(it->first.c_str())) {
          watcher->AddWatch(it->first.c_str());
          fcount++;
        }
      } else {
        if (!watcher2->IsWatched(it->first.c_str())) {
          watcher2->AddWatch(it->first.c_str());
          fcount++;
        }
      }
  }
}

void UnWatch(FSWatcher* watcher, int num) {
  int i = 0;
  UTTestFiles::iterator it = files.begin();
  for (; it != files.end() && i < num; ++it) {
    if (watcher->IsWatched(it->first.c_str())) {
      watcher->RemoveWatch(it->first.c_str());
      modify_stamp[it->first.c_str()] = 2;
      update_stamp[it->first.c_str()] = 1;
    }
    i++;
  }
  all -= num;
}

void Touch() {
  char str[10] = {"test\n"};
  UTTestFiles::iterator it = files.begin();
  for (; it != files.end(); ++it) {
    DEBUG_LOG(Info, "Now updating %s", it->first.c_str());
    FILE* fp = os::FOpen(it->first.c_str(), "wb");
    fwrite(str, sizeof(char), strlen(str), fp);
    os::FClose(fp);
  }
}

void Check() {
  UTTestFiles::iterator it = files.begin();
  for (; it != files.end(); ++it) {
    UTTestStamp::iterator m_it = modify_stamp.find(it->first.c_str());
    UTTestStamp::iterator u_it = update_stamp.find(it->first.c_str());
    if (m_it != modify_stamp.end()) {
      ASSERT_EQ(true, m_it->second == 2);
    } else {
      FAIL();
    }
    if (u_it != update_stamp.end()) {
      ASSERT_EQ(true, u_it->second == 1);
    } else {
      FAIL();
    }
  }
}

void modify(FSEvent* e) {
  DEBUG_LOG(Info, "modify %s", e->filename());
  {
    lock_guard<mutex> lock(mt);
    modify_stamp[e->filename()] = 2;
    count++;
  }
  os::Sleep(2000);
}
void update(FSEvent* e) {
  DEBUG_LOG(Info, "update %s", e->filename());
  {
    lock_guard<mutex> lock(mt);
    update_stamp[e->filename()] = 1;
    count++;
  }
  os::Sleep(2000);
}
}

TEST(FSEventTest, FSWatcherNormalTest) {
  using namespace os;
  Worker::default_worker_limit = 100;
  Logging::Initialize(stdout);
  Init();
  FSWatcher watcher;
  os::Sleep(500);
  Watch(&watcher);
  watcher.AddListener(FSWatcher::kModify, modify);
  watcher.AddListener(FSWatcher::kUpdate, update);
  watcher.RunAsync();
  os::Sleep(1500);
  Touch();
  while (count < (all * 2)) {os::Sleep(1000);}
  Check();
  CleanUp();
  watcher.Exit();
}

TEST(FSEventTest, FSWatcherMultiTest) {
  using namespace os;
  Init();
  FSWatcher watcher;
  FSWatcher watcher2;
  os::Sleep(500);
  WatchMulti(&watcher, &watcher2);
  watcher.AddListener(FSWatcher::kModify, modify);
  watcher.AddListener(FSWatcher::kUpdate, update);
  watcher2.AddListener(FSWatcher::kModify, modify);
  watcher2.AddListener(FSWatcher::kUpdate, update);
  watcher.RunAsync();
  watcher2.RunAsync();
  os::Sleep(1500);
  Touch();
  while (count < (all * 2)) {os::Sleep(1000);}
  Check();
  CleanUp();
  watcher.Exit();
  watcher2.Exit();
}

TEST(FSEventTest, FSWatcherAddTest) {
  using namespace os;
  all = 0;
  count = 0;
  files.clear();
  update_stamp.clear();
  modify_stamp.clear();
  Init(0, 1);
  FSWatcher watcher;
  os::Sleep(500);
  Watch(&watcher);
  watcher.AddListener(FSWatcher::kModify, modify);
  watcher.AddListener(FSWatcher::kUpdate, update);
  watcher.RunAsync();
  os::Sleep(500);
  Init(1, 2);
  os::Sleep(1000);
  Watch(&watcher);
  os::Sleep(1000);
  Touch();
  while (count < (all * 2)) {os::Sleep(1000);}
  Check();
  CleanUp();
  watcher.Exit();
}

TEST(FSEventTest, FSWatcherRemoveTest) {
  using namespace os;
  Logging::Initialize(stdout);
  all = 0;
  count = 0;
  files.clear();
  update_stamp.clear();
  modify_stamp.clear();
  Init(0, 2);
  FSWatcher watcher;
  os::Sleep(500);
  Watch(&watcher);
  watcher.AddListener(FSWatcher::kModify, modify);
  watcher.AddListener(FSWatcher::kUpdate, update);
  watcher.RunAsync();
  os::Sleep(500);
  UnWatch(&watcher, 1);
  os::Sleep(1000);
  Touch();
  while (count < (all * 2)) {os::Sleep(1000);}
  Check();
  CleanUp();
  watcher.Exit();
}

