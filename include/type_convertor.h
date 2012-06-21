#ifndef INCLUDE_TYPE_CONVERTOR_H_
#define INCLUDE_TYPE_CONVERTOR_H_
#include "lib/type_traits.h"
namespace os {

template <typename T>
class TypeConvertor {
 public :
  typedef typename add_reference<T>::type RefType;
  typedef typename add_pointer<T>::type PtrType;
  static RefType ToRef(RefType t) {return t;}
  static PtrType ToPointer(PtrType t) {return t; }
};

}

#endif
