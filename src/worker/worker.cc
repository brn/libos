#include <worker/worker.h>
#include <logging.h>
namespace os {

Worker::Worker(int max) : channel_(max){};

Worker* Worker::default_worker() {
  if (flag_ == 0) {
    ++flag_;
    default_worker_ = new Worker(default_worker_limit);
    atexit(Remove);
  }
  return default_worker_;
}

void Worker::Remove() {
  delete default_worker_;
}
int Worker::default_worker_limit = 50;
atomic_count Worker::flag_(0);
Worker* Worker::default_worker_;
}
