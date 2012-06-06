
#ifndef UTILS_SMART_POINTER_COMMON_PTR_DELETER_H_
#define UTILS_SMART_POINTER_COMMON_PTR_DELETER_H_

#include <stdio.h>
#include <stdlib.h>
#include "../../utilities.h"

namespace os {
  
  template <typename T>
  struct PtrDeleter : private Static {
    inline static void Delete (T* ptr ) {
      delete ptr;
    };
    inline static void DeleteArray (T* ptr) {
      delete [] ptr;
    }
    inline static void Free (T* ptr) {
      free (ptr);
    }
  };
};

#endif


