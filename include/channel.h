#ifndef LIBOS_THREAD_CHANNEL_H_
#define LIBOS_THREAD_CHANNEL_H_
#include <deque>
#include "thread.h"
#include "lib/shared_ptr.h"
#include "worker_count.h"
#include "worker_queue.h"
namespace os {
class Channel {
  typedef shared_ptr<thread> ThreadHandle;
  typedef std::deque<ThreadHandle> Workers;
 public :
  Channel();
  ~Channel();
  template <typename T>
  void send_request(T req) {
    worker_queue_.set_request(req);
    cond_.notify_all();
  }
 private :
  void Initialize();
  void CreateWorker(int i);
  WorkerQueue::Request Wait();
  void Run(int id, bool additional);
  WorkerCount worker_count_;
  Workers workers_;
  WorkerQueue worker_queue_;
  condition_variable cond_;
  mutex mutex_;
};

}

#endif
