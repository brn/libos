#ifndef INCLUDE_CALLBACKS_IMPL_H_
#define INCLUDE_CALLBACKS_IMPL_H_

#include "../thread.h"
#include "../defines.h"
#include "../lib/bind.h"
#include "../lib/make_shared.h"

namespace os {
#define TEMPLATE template <typename Function, typename ListenerContainer>

TEMPLATE
ALWAYS_INLINE Callbacks<Function, ListenerContainer>::Callbacks(){}

TEMPLATE
ALWAYS_INLINE Callbacks<Function, ListenerContainer>::~Callbacks(){}

TEMPLATE
template <typename Listener>
ALWAYS_INLINE void Callbacks<Function, ListenerContainer>::Add(Listener listener) {
  container_.push_back(make_shared<EventListener>(listener));
}

TEMPLATE
template <typename Listener>
ALWAYS_INLINE void Callbacks<Function, ListenerContainer>::operator += (Listener listener) {
  AddListener(listener);
}

TEMPLATE
ALWAYS_INLINE void Callbacks<Function, ListenerContainer>::Remove() {
  container_.clear();
}

#define SIMPLE_NOTIFICATOR_ITER typename Callbacks<Function, ListenerContainer>::iterator

TEMPLATE
ALWAYS_INLINE SIMPLE_NOTIFICATOR_ITER Callbacks<Function, ListenerContainer>::begin() {
  return container_.begin();
}

TEMPLATE
ALWAYS_INLINE SIMPLE_NOTIFICATOR_ITER Callbacks<Function, ListenerContainer>::end() {
  return container_.end();
}

TEMPLATE
ALWAYS_INLINE SIMPLE_NOTIFICATOR_ITER Callbacks<Function, ListenerContainer>::erase(SIMPLE_NOTIFICATOR_ITER it) {
  return container_.erase(it);
}

#undef SIMPLE_NOTIFICATOR_ITER

#define TEMPLATE_ARGUMENTS(z, n, in)                                  \
  template<typename Function, typename ListenerContainer>               \
  template<BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), typename T)>      \
  in void Callbacks<Function, ListenerContainer>::Invoke(BOOST_PP_ENUM_BINARY_PARAMS_Z(z, BOOST_PP_INC(n), T, t)) { \
    for (typename ListenerContainer::iterator it = container_.begin(); it != container_.end(); ++it) { \
      (**it)(BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), t));            \
    }                                                                   \
  }                                                                     \

BOOST_PP_REPEAT(11, TEMPLATE_ARGUMENTS, ALWAYS_INLINE);

#undef TEMPLATE_ARGUMENTS


#define TEMPLATE_ARGUMENTS(z, n, in)                                  \
  template<typename Function, typename ListenerContainer>               \
  template<BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), typename T)>      \
  in void Callbacks<Function, ListenerContainer>::InvokeAsync(BOOST_PP_ENUM_BINARY_PARAMS_Z(z, BOOST_PP_INC(n), T, t)) { \
    for (typename ListenerContainer::iterator it = container_.begin(); it != container_.end(); ++it) { \
      thread th(bind((**it), BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), t))); \
      th.detach();                                                      \
    }                                                                   \
  }

BOOST_PP_REPEAT(11, TEMPLATE_ARGUMENTS, ALWAYS_INLINE);

#undef TEMPLATE_ARGUMENTS


#define TEMPLATE_ARGUMENTS(z, n, in)                                  \
  template<typename Function, typename ListenerContainer>               \
  template<BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), typename T)>      \
  in void Callbacks<Function, ListenerContainer>::operator()(BOOST_PP_ENUM_BINARY_PARAMS_Z(z, BOOST_PP_INC(n), T, t), bool async) { \
    if (async) {                                                        \
      NotifyAsync(BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), t));       \
    } else {                                                            \
      Notify(BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), t));            \
    }                                                                   \
  }

BOOST_PP_REPEAT(11, TEMPLATE_ARGUMENTS, ALWAYS_INLINE);

#undef TEMPLATE_ARGUMENTS


TEMPLATE
ALWAYS_INLINE void Callbacks<Function, ListenerContainer>::swap(Callbacks<Function, ListenerContainer>& notificator) {
  container_.swap(notificator.container_);
}

#undef TEMPLATE
}

#endif
