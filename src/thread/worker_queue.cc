#include <worker_queue.h>

namespace os {

WorkerQueue::WorkerQueue(){}
WorkerQueue::~WorkerQueue(){}

WorkerQueue::Request WorkerQueue::pop_request() {
  Request req = queue_.front();
  queue_.pop_front();
  return req;
}

}
