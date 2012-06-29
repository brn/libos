#include <worker/worker_count.h>
#include <thread.h>
namespace os {
static const int thread_default_count = 4;
int GetThreadCount() {
  if (thread::hardware_concurrency() >= thread_default_count) {
    return thread::hardware_concurrency();
  }
  return thread_default_count;
}
WorkerCount::WorkerCount(int limit)
    : limit_(limit),
      current_thread_count_(GetThreadCount()),
      running_thread_count_(0){}

WorkerCount::~WorkerCount(){}

int WorkerCount::current_thread_count() const {return static_cast<long>(current_thread_count_);}

int WorkerCount::running_thread_count() const {return static_cast<long>(running_thread_count_);}

void WorkerCount::add_thread_count() {++current_thread_count_;}

void WorkerCount::sub_thread_count() {--current_thread_count_;}

void WorkerCount::add_running_thread_count() {++running_thread_count_;}

void WorkerCount::sub_running_thread_count() {--running_thread_count_;}

bool WorkerCount::limit() const {return current_thread_count_ == limit_;}

}
