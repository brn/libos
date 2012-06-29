#ifndef LIBOS_THREAD_CHANNEL_H_
#define LIBOS_THREAD_CHANNEL_H_
#include <deque>
#include "logging.h"
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
  Channel(int limit);
  ~Channel();
  template <typename T>
  void send_request(T req) {
    if (worker_count_.running_thread_count() >= (worker_count_.current_thread_count() - 1)) {
      if (!worker_count_.limit()) {
        worker_count_.add_thread_count();
        CreateWorker(static_cast<int>(worker_count_.running_thread_count() + 1), true);
      }
    }
    worker_queue_.set_request(req);
    cond_.notify_all();
  }
 private :
  void Initialize();
  void CreateWorker(int i, bool additional);
  WorkerQueue::Request Wait(bool);
  void Run(int id, bool additional);
  void ProcessRequest(const WorkerQueue::Request &fn, int id);
  bool exit_;
  WorkerCount worker_count_;
  Workers workers_;
  WorkerQueue worker_queue_;
  condition_variable cond_;
  mutex mutex_;
  mutex req_mutex_;
};

}

#endif
