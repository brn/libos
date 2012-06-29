#ifndef LIBOS_THREAD_WORKER_H_
#define LIBOS_THREAD_WORKER_H_
#include "channel.h"
#include "thread.h"
#include "lib/bind.h"
#include "lib/atomic.h"
namespace os {
class Worker {
 public :
  static int default_worker_limit;
  Worker(){};
  Worker(int max);
  ~Worker(){};
  static Worker* default_worker();
  template <typename T>
  void send_request(T);
  template <typename T>
  void send_requests(T);
 private :
  static void Remove();
  Channel channel_;
  mutex mutex_;
  static atomic_count flag_;
  static Worker* default_worker_;
};

}
#include "worker-impl.h"
#endif
