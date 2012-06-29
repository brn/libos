#ifndef LIBOS_THREAD_WORKER_QUEUE_H_
#define LIBOS_THREAD_WORKER_QUEUE_H_
#include <deque>
#include <lib/function.h>
namespace os {
class WorkerQueue {
 public :
  typedef function<void ()> Request;
  WorkerQueue();
  ~WorkerQueue();
  template <typename T>
  void set_request(T request) {
    Request fn = request;
    queue_.push_back(fn);
  }
  Request pop_request();
  bool empty() const {return queue_.empty();}
 private :
  typedef std::deque<Request> Queue;
  Queue queue_;
};
}

#endif
