#ifndef UTILS_SMART_POINTER_SCOPED_LIST_H_
#define UTILS_SMART_POINTER_SCOPED_LIST_H_
#include <list>
#include <utility>
#include "../utilities.h"
#include "common/ptr_deleter.h"
#include "shared_ptr.h"

namespace os {

template <typename T>
class ScopedList : private Uncopyable {
 public :
  inline ScopedList ();
  virtual ~ScopedList ();
  
  template <typename Class, typename Deleter>
  inline T* Retain (Class* ptr, Deleter deleter);
  
  template <typename Deleter>
  inline T* Retain (Deleter deleter);
  
  template <typename Class>
  inline T* Retain(Class* ptr);
  
  template <typename Class>
  inline T* Retain();
  
 protected :
  inline T* RegistToList_(SharedPtr<T> handle, T* ptr);
  typedef std::list<SharedPtr<T> > List_;
  List_ list_;
 private :
  inline static void* operator new (size_t size){return 0;};
  inline static void* operator new [] (size_t size){return 0;};
};

template <typename T>
class ScopedArrayList : public ScopedList<T> {
 public :
  inline ScopedArrayList();
  template <typename Class>
  inline T* Retain(Class* ptr);
};

template <typename T>
class ScopedAllocaterList : public ScopedList<T> {
 public :
  inline ScopedAllocaterList();
  template <typename Class>
  inline T* Retain(Class* ptr);
};

}

#include <smart_pointer/scoped_list-impl.h>

namespace os {
typedef ScopedArrayList<char> ScopedStrList;
typedef ScopedArrayList<wchar_t> ScopedWStrList;
typedef ScopedAllocaterList<char> ScopedCStrList;
typedef ScopedAllocaterList<wchar_t> ScopedCWStrList;
}

#endif
