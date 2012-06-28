#include <worker_count.h>
#include <thread.h>
namespace os {
static const int thread_default_count = 4;
WorkerCount::WorkerCount() {
  if (thread::hardware_concurrency() > 3) {
    count_ = thread::hardware_concurrency();
  } else {
    count_ = thread_default_count;
  }
}
WorkerCount::~WorkerCount(){}

int WorkerCount::count() const {return count_;}

}
