
#ifndef UTILS_SMART_POINTER_REFCOUNT_BASE_H_
#define UTILS_SMART_POINTER_REFCOUNT_BASE_H_
#include "../utilities.h"
namespace os {
class RefCountBase : private Uncopyable {
 public :
  RefCountBase (int num = 0) : count_(num) {};
  virtual ~RefCountBase (){};
  inline void Add() { Atomic::Increment(&count_); };
  inline void Release() {
    if (Atomic::Decrement(&count_) == 0) {
      delete this;
    }
  };
 private :
  AtomicWord count_;
};
}


#endif


