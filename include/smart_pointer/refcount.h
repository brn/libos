
#ifndef UTILS_SMART_POINTER_REF_COUNT_H_
#define UTILS_SMART_POINTER_REF_COUNT_H_

#include "refcount_base.h"
#include "common/ptr_handle.h"


namespace os {
/**
 *@class
 *@extends
 *RefCountBase
 *@description
 *Manage reference count and pointer.
 */
template <typename T>
class RefCount : public RefCountBase {
    
 public:
    
  /*
   *@constructor
   *Normal constructor.
   */
  inline explicit RefCount (T *target);
    
  /*
   *@constructor
   *Has deleter constructor.
   */
  template <typename Deleter>
  inline RefCount (T *target, Deleter deleter );


  /*
   *@constructor
   *Covert constructor.
   */
  inline explicit RefCount (PtrHandleBase* base);
  
  /*
   *@destructor
   *Normal destructor.
   */
  inline ~RefCount ();

 private :
    
  /**
   *@private
   *@description
   *This pointer type is RefPtrBase,
   *but real type is upcast pointer of RefPtr or RefPtrDeleter.
   */
  PtrHandleBase *ptr_handle_;
    
};
}

#include "refcount-impl.h"

#endif

