#include <fs/event/fs_watcher.h>
#include <fs.h>
namespace os { namespace fs {
FSWatcher::FSWatcher()
    : watcher_(new FSWatcherPlt()){}

FSWatcher::~FSWatcher() {delete FSWatcherPlt;}
void FSWatcher::AddWatch(const char* path) {
  watcher_->AddWatch(path);
}

void FSWatcher::RemoveWatch(const char* path) {
  watcher_->RemoveWatch(path);
}

void FSWatcher::RemoveWatch() {
  watcher_->RemoveWatch();
}

void FSWatcher::Run() {
  watcher_->Run();
}

void FSWatcher::RunAsync() {
  watcher_->RunAsync();
}

void FSWatcher::Exit() {
  watcher_->Exit();
}

bool FSWatcher::IsRunning() {
  return watcher_->IsRunning();
}

bool FSWatcher::IsWatched (const char* path) {
  return watcher_->IsWatched(path);
}

const char FSWatcher::kModify[] = FSWatcherPlt::kModify;
const char FSWatcher::kUpdate[] = FSWatcherPlt::kUpdate;
const char FSWatcher::kDelete[] = FSWatcherPlt::kDelete;
}}
