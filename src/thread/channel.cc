#include <channel.h>
#include <lib/make_shared.h>
#include <lib/bind.h>
#include <logging.h>
namespace os {

Channel::Channel() {Initialize();}
Channel::~Channel(){}

void Channel::Initialize() {
  for (int i = 0; i < worker_count_.count(); i++) {
    CreateWorker(i);
  }
}

WorkerQueue::Request Channel::Wait() {
  unique_lock<mutex> lock(mutex_);
  while (worker_queue_.empty()) {
    cond_.wait(lock);
  }
  return worker_queue_.pop_request();
}

void Channel::CreateWorker(int i) {
  ThreadHandle thread_handle = make_shared<thread>(bind(&Channel::Run, this, i, false));
  thread_handle->detach();
  workers_.push_back(thread_handle);
}

void Channel::Run(int id, bool additional) {
  while (!additional && id < 13) {
    WorkerQueue::Request fn = Wait();
    DEBUG_LOG(Info, "thread %d begin process.", id);
    fn();
  }
}

}
