#ifndef LIBOS_THREAD_WORKER_COUNT_H_
#define LIBOS_THREAD_WORKER_COUNT_H_

namespace os {
class WorkerCount {
 public :
  WorkerCount();
  ~WorkerCount();
  int count() const;
 private :
  int count_;
};
}

#endif
