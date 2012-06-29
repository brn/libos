#ifndef LIBOS_THREAD_WORKER_IMPL_H_
#define LIBOS_THREAD_WORKER_IMPL_H_
namespace os {

template <typename T>
void Worker::send_request(T req) {
  lock_guard<mutex> lock(mutex_);
  channel_.send_request(req);
}

template <typename T>
void Worker::send_requests(T reqs) {
  lock_guard<mutex> lock(mutex_);
  std::for_each(reqs.begin(), reqs.end(), bind<void (T)>(&Channel::send_request, channel_, placeholders::_1));
}
}

#endif
