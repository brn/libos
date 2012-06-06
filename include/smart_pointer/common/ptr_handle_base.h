
#ifndef UTILS_SMART_POINTER_COMMON_PTR_HANDLE_BASE_H_
#define UTILS_SMART_POINTER_COMMON_PTR_HANDLE_BASE_H_

#include "../../utilities.h"

namespace os {
  class PtrHandleBase : private Uncopyable {
  public :
    inline PtrHandleBase(){}
    inline virtual ~PtrHandleBase () {};
    virtual void Dispose () = 0;
  };
}

#endif

