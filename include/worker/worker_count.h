#ifndef LIBOS_THREAD_WORKER_COUNT_H_
#define LIBOS_THREAD_WORKER_COUNT_H_
#include "lib/atomic.h"
namespace os {
class WorkerCount {
 public :
  WorkerCount(int limit);
  ~WorkerCount();
  int current_thread_count() const;
  int running_thread_count() const;
  void add_thread_count();
  void sub_thread_count();
  void add_running_thread_count();
  void sub_running_thread_count();
  bool limit() const;
 private :
  int limit_;
  atomic_count current_thread_count_;
  atomic_count running_thread_count_;
};
}

#endif
