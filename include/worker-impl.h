#ifndef LIBOS_THREAD_WORKER_IMPL_H_
#define LIBOS_THREAD_WORKER_IMPL_H_
namespace os {

template <typename T>
void Worker::send_request(T req) {channel_.send_request(req);}

template <typename T>
void Worker::send_requests(T reqs) {
  std::for_each(reqs.begin(), reqs.end(), bind(&Channel::send_request, channel_, placeholders::_1));
}
}

#endif
