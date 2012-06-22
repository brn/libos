#include <thread_pool.h>

namespace os {

ThreadPool::ThreadPool()
    : num_(thread::hardware_concurrency()){}

ThreadPool::ThreadPool(int num)
    : num_(num) {}

ThreadPool::~ThreadPool(){}



}
