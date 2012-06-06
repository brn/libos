#ifndef UTILS_SMART_POINTER_COMMON_PTR_HANDLE_IMPL_H_
#define UTILS_SMART_POINTER_COMMON_PTR_HANDLE_IMPL_H_

namespace os {

template <typename T, typename Deleter>
inline PtrHandleDeleter<T,Deleter>::PtrHandleDeleter (T* ptr, Deleter deleter)
    : PtrHandleBase(), ptr_ (ptr), deleter_ (deleter) {};

template <typename T, typename Deleter>
inline T* PtrHandleDeleter<T,Deleter>::Get () {
  return ptr_;
}

template <typename T, typename Deleter>
inline void PtrHandleDeleter<T,Deleter>::Dispose () {
  if (ptr_ != 0) {
    deleter_(ptr_);
    ptr_ = 0;
  }
}


template <typename T>
inline PtrHandle<T>::PtrHandle(T* ptr) : ptr_ (ptr){};

template <typename T>
inline T* PtrHandle<T>::Get () {
  return ptr_;
}

template <typename T>
inline void PtrHandle<T>::Dispose () {
  if (ptr_ != 0) {
    PtrDeleter<T>::Delete(ptr_);
    ptr_ = 0;
  }
}

}

#endif
