
#ifndef UTILS_SMART_POINTER_COMMON_PTR_TRAIS_H_
#define UTILS_SMART_POINTER_COMMON_PTR_TRAIS_H_

namespace os {

  /**
   *@class
   *Type traits.
   *Distinct void and other types,
   *becase of reference type of void is cause error.
   */
  template <typename T>
  struct PtrTraits {
    typedef T& types;
  };

  //Specializing for type of void. 
  template <>
  struct PtrTraits<void> {
    typedef void types;
  };

}

#endif

