#ifndef INCLUDE_TYPE_CONVERTOR_H_
#define INCLUDE_TYPE_CONVERTOR_H_
#include "lib/type_traits.h"
namespace os {
template <typename T, typename RefType, typename PtrType>
class Deref_{
 public :
  static RefType GetRef(T* ptr) {
    return *ptr;
  }

  static RefType GetRef(T& ref) {
    return ref;
  }

  static PtrType GetPtr(T* ptr) {
    return ptr;
  }

  static PtrType GetPtr(T& ref) {
    return &ref;
  }
};

template <typename T>
class TypeConvertor {
 public :
  typedef typename remove_reference<typename remove_pointer<T>::type>::type BaseType;
  typedef typename add_reference<BaseType>::type RefType;
  typedef typename add_pointer<BaseType>::type PtrType;
  static RefType ToRef(T t) {
    return Deref_<BaseType,
                  RefType,
                  PtrType>::GetRef(t);
  }
  static PtrType ToPointer(T t) {
    return Deref_<BaseType,
                  RefType,
                  PtrType>::GetPtr(t);
  }
};

}

#endif
