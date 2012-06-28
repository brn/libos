#include <worker.h>
#include <logging.h>
namespace os {

static const int thread_default_count = 4;

Worker::Worker() {
  Initialize();
}

Worker::~Worker() {}

void Worker::Initialize() {
  InitializeWorkers();  
}

void Worker::InitializeWorkers() {
  for (int i = 0; i < worker_count_.count(); i++) {
    CreateWorker(i);
  }
  DEBUG_LOG(Info, "thread count => %d", thread_count_);
}

void Worker::CreateWorker(int i) {
  ThreadHandle thread_handle = make_shared<thread>(bind(&Worker::Run, this, i, false));
  thread_handle.detach();
  workers_.push_back(thread_handle);
}

Worker::Function Worker::Wait() {
  unique_lock<mutex> lock(mutex_);
  while (queue_.empty()) {
    cond_.wait(lock);
  }
  return queue_.pop_request();
}

void Worker::Run(int id, bool additional) {
  while (1) {
    WorkerQueue::Req fn = Wait();
    DEBUG_LOG(Info, "thread %d begin process.", id);
    fn();
  }
}

}
