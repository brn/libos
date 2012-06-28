#ifndef LIBOS_THREAD_WORKER_H_
#define LIBOS_THREAD_WORKER_H_
#include "channel.h"
#include "lib/bind.h"
namespace os {

class Worker {
 public :
  Worker(){};
  ~Worker(){};
  template <typename T>
  void send_request(T);
  template <typename T>
  void send_requests(T);
 private :
  Channel channel_;
};

}
#include "worker-impl.h"
#endif
