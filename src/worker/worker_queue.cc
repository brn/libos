#include <worker/worker_queue.h>

namespace os {

WorkerQueue::WorkerQueue(){}
WorkerQueue::~WorkerQueue(){}

WorkerQueue::Request WorkerQueue::pop_request() {
  if (!queue_.empty()) {
    Request req = queue_.front();
    queue_.pop_front();
    return req;
  }
  return Request();
}

}
