
#ifndef UTILS_SMART_POINTER_COMMON_PTR_HANDLE_H_
#define UTILS_SMART_POINTER_COMMON_PTR_HANDLE_H_

#include <stdio.h>
#include "ptr_handle_base.h"
#include "ptr_deleter.h"

namespace os {

template <typename T, typename Deleter>
class PtrHandleDeleter : public PtrHandleBase {
 public :
  inline explicit PtrHandleDeleter(T* ptr, Deleter deleter);
  inline T* Get();
  inline void Dispose();
 private :
  T *ptr_;
  Deleter deleter_;
};

template <typename T>
class PtrHandle : public PtrHandleBase {
 public :
  inline explicit PtrHandle(T* ptr);
  inline T* Get();
  inline void Dispose();
 private :
  T *ptr_;
};

}

#include "ptr_handle-impl.h"

#endif

