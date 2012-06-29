#include <worker/channel.h>
#include <lib/make_shared.h>
#include <lib/bind.h>
#include <logging.h>
#include <lib/foreach.h>
namespace os {

Channel::Channel()
    : exit_(false),
      worker_count_(12) {Initialize();}

Channel::Channel(int limit)
    : exit_(false),
      worker_count_(limit) {Initialize();}

Channel::~Channel() {
  exit_ = true;
  cond_.notify_all();
  while (worker_count_.current_thread_count()){}
}

inline void Channel::Initialize() {
  for (int i = 0; i < worker_count_.current_thread_count(); i++) {
    CreateWorker(i, false);
  }
}

inline WorkerQueue::Request Channel::Wait(bool additional) {
  unique_lock<mutex> lock(mutex_);
  int count = 0;
  while (worker_queue_.empty() && !exit_) {
    if (additional) {
      if (count > 0) {
        return WorkerQueue::Request();
      }
      count++;
      wait_for(cond_, lock, 5000);
    } else {
      cond_.wait(lock);
    }
  }
  return worker_queue_.pop_request();
}

inline void Channel::CreateWorker(int i, bool additional) {
  ThreadHandle thread_handle = make_shared<thread>(bind(&Channel::Run, this, i, additional));
  thread_handle->detach();
  workers_.push_back(thread_handle);
}

inline void Channel::Run(int id, bool additional) {
  this_thread::disable_interruption di;
  while (1) {
    WorkerQueue::Request fn = Wait(additional);
    if (fn) {
      DEBUG_LOG(Info, "all running thread count -> %d",  worker_count_.running_thread_count());
      ProcessRequest(fn, id);
    } else {
      worker_count_.sub_thread_count();
      return;
    }
  }
}

inline void Channel::ProcessRequest(const WorkerQueue::Request &fn, int id) {
  worker_count_.add_running_thread_count();
  DEBUG_LOG(Info, "thread %d begin process.", id);
  fn();
  worker_count_.sub_running_thread_count();
}

}
