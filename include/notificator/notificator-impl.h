#ifndef UTILS_NOTIFICATOR_IMPL_H_
#define UTILS_NOTIFICATOR_IMPL_H_
/**
 *@author Taketoshi Aono
 *@fileOverview
 *@license
 *Copyright (c) 2011 Taketoshi Aono
 *Licensed under the BSD.
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 *associated doc umentation files (the "Software"), to deal in the Software without restriction,
 *including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 *subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all copies or
 *substantial portions ofthe Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 *TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 *CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *DEALINGS IN THE SOFTWARE.
 */
#include "../thread.h"
#include "../lib/bind.h"
#include "../lib/make_shared.h"
#include "../lib/foreach.h"
#include "../logging.h"
#include "../static_assert.h"
#include "../defines.h"

namespace os {

#define TEMPLATE template<typename Signature>

TEMPLATE
ALWAYS_INLINE Notificator<Signature>::Notificator() {}

TEMPLATE
ALWAYS_INLINE Notificator<Signature>::Notificator(const Notificator<Signature>& notificator) {
  listeners_ = notificator.listeners_;
}

TEMPLATE
ALWAYS_INLINE const Notificator<Signature>& Notificator<Signature>::operator = (const Notificator<Signature>& notificator) {
  listeners_ = notificator.listeners_;
  return (*this);
}

TEMPLATE
template <typename Listener>
ALWAYS_INLINE void Notificator<Signature>::AddListener(const char* key, Listener listener) {
  //Listener adapter is allocated as the heap object,
  //because this object treat as the base class type ListenerAdapterBase.
  //Object lifetime is controlled by the shared_ptr
  //so a notificator instance that create ListenerAdapter is destroyed,
  //ListenerAdapter is destroyed too.
  listeners_.insert(ListenerSet(key, make_shared<ListenerFunction>(listener)));
}

TEMPLATE
template <typename Listener>
ALWAYS_INLINE void Notificator<Signature>::operator += (std::pair<const char*, Listener> listener_pack) {
  AddListener(listener_pack.first, listener_pack.second);
}

TEMPLATE
ALWAYS_INLINE void Notificator<Signature>::RemoveListener(const char* key) {
  ListenersIterator it = listeners_.find(key);
  if (it != listeners_.end()) {
    listeners_.erase(it);
  }
}

TEMPLATE
ALWAYS_INLINE void Notificator<Signature>::operator -= (const char* key) {
  RemoveListener(key);
}


#define TEMPLATE_ARGUMENTS(z, n, in)                                    \
  template<typename Signature>                                          \
  template<BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), typename T)>      \
  in void Notificator<Signature>::NotifyAll(BOOST_PP_ENUM_BINARY_PARAMS_Z(z, BOOST_PP_INC(n), T, t)) { \
    DEBUG_LOG(Info, "Notificator::NotifyAll Called");                   \
    foreach (typename Listeners::value_type& it, listeners_) { \
      (*(it.second))(BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), t));   \
    }                                                                   \
  }

BOOST_PP_REPEAT(11, TEMPLATE_ARGUMENTS, ALWAYS_INLINE);

#undef TEMPLATE_ARGUMENTS

#define TEMPLATE_ARGUMENTS(z, n, in)                                    \
  template<typename Signature>                                          \
  template<BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), typename T)>      \
  in void Notificator<Signature>::NotifyAllAsync(BOOST_PP_ENUM_BINARY_PARAMS_Z(z, BOOST_PP_INC(n), T, t)) { \
    DEBUG_LOG(Info, "Notificator::NotifyAllAsync Called");              \
    foreach (typename Listeners::value_type& it, listeners_) {          \
      workers_.send_request(bind(*(it.second), BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), t))); \
    }                                                                   \
  }

BOOST_PP_REPEAT(11, TEMPLATE_ARGUMENTS, ALWAYS_INLINE);

#undef TEMPLATE_ARGUMENTS


#define TEMPLATE_ARGUMENTS(z, n, in)                                    \
  template<typename Signature>                                          \
  template<BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), typename T)>      \
  in void Notificator<Signature>::NotifyForKey(const char* key, BOOST_PP_ENUM_BINARY_PARAMS_Z(z, BOOST_PP_INC(n), T, t)) { \
    DEBUG_LOG(Info, "Notificator::NotifyForKey[%s]", key);              \
    ListenersRange listener_range = listeners_.equal_range(key);        \
    foreach (typename Listeners::value_type& it, listener_range) { \
      (*(it.second))(BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), t));   \
    }                                                                   \
  }

BOOST_PP_REPEAT(11, TEMPLATE_ARGUMENTS, ALWAYS_INLINE);

#undef TEMPLATE_ARGUMENTS

#define TEMPLATE_ARGUMENTS(z, n, in)                                    \
  template<typename Signature>                                          \
  template<BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), typename T)>      \
  in void Notificator<Signature>::NotifyForKeyAsync(const char* key, BOOST_PP_ENUM_BINARY_PARAMS_Z(z, BOOST_PP_INC(n), T, t)) { \
    DEBUG_LOG(Info, "Notificator::NotifyForKeyAsync[%s] Called", key);  \
    ListenersRange listener_range = listeners_.equal_range(key);        \
    foreach (typename Listeners::value_type& it, listener_range) { \
      workers_.send_request(bind(*(it.second), BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), t))); \
    }                                                                   \
  }

BOOST_PP_REPEAT(11, TEMPLATE_ARGUMENTS, ALWAYS_INLINE);

#undef TEMPLATE_ARGUMENTS


#define TEMPLATE_ARGUMENTS(z, n, in)                                    \
  template<typename Signature>                                          \
  template<BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), typename T)>      \
  in void Notificator<Signature>::operator()(BOOST_PP_ENUM_BINARY_PARAMS_Z(z, BOOST_PP_INC(n), T, t), bool async) { \
    if (async) {                                                        \
      NotifyAllAsync(BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), t));    \
    } else {                                                            \
      NotifyAll(BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), t));         \
    }                                                                   \
  }

BOOST_PP_REPEAT(11, TEMPLATE_ARGUMENTS, ALWAYS_INLINE);

#undef TEMPLATE_ARGUMENTS


#define TEMPLATE_ARGUMENTS(z, n, in)                                    \
  template<typename Signature>                                          \
  template<BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), typename T)>      \
  in void Notificator<Signature>::operator()(const char* key, BOOST_PP_ENUM_BINARY_PARAMS_Z(z, BOOST_PP_INC(n), T, t), bool async) { \
    if (async) {                                                        \
      NotifyForKeyAsync(key, BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), t)); \
    } else {                                                            \
      NotifyForKey(key, BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), t)); \
    }                                                                   \
  }

BOOST_PP_REPEAT(11, TEMPLATE_ARGUMENTS, ALWAYS_INLINE);

#undef TEMPLATE_ARGUMENTS


TEMPLATE
ALWAYS_INLINE void Notificator<Signature>::swap(Notificator<Signature>& notificator) {
  listeners_.swap(notificator.listeners_);
}

#undef TEMPLATE

}

#endif
