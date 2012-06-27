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
#include "../lib/foreach.h"
#include "../lib/make_shared.h"
#include "../lib/ref.h"
#include "../logging.h"
#include "../static_assert.h"
namespace os {

#define TEMPLATE template<typename Event>

TEMPLATE
inline Notificator<Event>::Notificator() {}

TEMPLATE
inline Notificator<Event>::Notificator(const Notificator<Event>& notificator) {
  listeners_ = notificator.listeners_;
}

TEMPLATE
inline const Notificator<Event>& Notificator<Event>::operator = (const Notificator<Event>& notificator) {
  listeners_ = notificator.listeners_;
  return (*this);
}

TEMPLATE
template <typename Listener>
inline void Notificator<Event>::AddListener(const char* key, Listener listener) {
  //Listener adapter is allocated as the heap object,
  //because this object treat as the base class type ListenerAdapterBase.
  //Object lifetime is controlled by the shared_ptr
  //so a notificator instance that create ListenerAdapter is destroyed,
  //ListenerAdapter is destroyed too.
  listeners_.insert(ListenerSet(key, make_shared<ListenerFunction>(listener)));
}

TEMPLATE
template <typename Listener>
inline void Notificator<Event>::operator += (std::pair<const char*, Listener> listener_pack) {
  AddListener(listener_pack.first, listener_pack.second);
}

TEMPLATE
inline void Notificator<Event>::RemoveListener(const char* key) {
  ListenersIterator it = listeners_.find(key);
  if (it != listeners_.end()) {
    listeners_.erase(it);
  }
}

TEMPLATE
inline void Notificator<Event>::operator -= (const char* key) {
  RemoveListener(key);
}


TEMPLATE
inline void Notificator<Event>::NotifyAll(Event e) {
  DEBUG_LOG(Info, "Notificator::NotifyAll Called");
  forEach(typename Listeners::value_type& it, listeners_) {
    (*(it.second))(e);
  }
}

TEMPLATE
inline void Notificator<Event>::NotifyAllAsync(Event e) {
  DEBUG_LOG(Info, "Notificator::NotifyAllAsync Called");
  forEach(ListenerSet& it, listeners_) {
    thread th(bind(*(it.second.get()), e));
    th.detach();
  }
}

TEMPLATE
inline void Notificator<Event>::NotifyForKey(const char* key, Event e) {
  DEBUG_LOG(Info, "Notificator::NotifyForKey[%s]", key);
  ListenersRange listener_range = listeners_.equal_range(key);
  //Call all liteners that identified by same key.
  forEach(typename Listeners::value_type& it, listener_range) {
    (*(it.second))(e);
  }
}

TEMPLATE
inline void Notificator<Event>::NotifyForKeyAsync(const char* key, Event e) {
  DEBUG_LOG(Info, "Notificator::NotifyForKeyAsync[%s] Called", key);
  ListenersRange listener_range = listeners_.equal_range(key);
  //Call all liteners that identified by same key.
  forEach(typename Listeners::value_type& it, listener_range) {
    thread th(bind(*(it.second.get()), e));
    th.detach();
  }
}

TEMPLATE
inline void Notificator<Event>::operator()(Event e, bool async) {
  if (async) {
    NotifyAllAsync(e);
  } else {
    NotifyAll(e);
  }
}

TEMPLATE
inline void Notificator<Event>::operator()(const char* key, Event e, bool async) {
  if (async) {
    NotifyForKeyAsync(key, e);
  } else {
    NotifyForKey(key, e);
  }
}

TEMPLATE
inline void Notificator<Event>::swap(Notificator<Event>& notificator) {
  listeners_.swap(notificator.listeners_);
}

#undef TEMPLATE
#define TEMPLATE template <typename Function, typename ListenerContainer>

TEMPLATE
Callbacks<Function, ListenerContainer>::Callbacks(){}

TEMPLATE
Callbacks<Function, ListenerContainer>::~Callbacks(){}

TEMPLATE
template <typename Listener>
void Callbacks<Function, ListenerContainer>::Add(Listener listener) {
  container_.push_back(make_shared<EventListener>(listener));
}

TEMPLATE
template <typename Listener>
void Callbacks<Function, ListenerContainer>::operator += (Listener listener) {
  AddListener(listener);
}

TEMPLATE
void Callbacks<Function, ListenerContainer>::Remove() {
  container_.clear();
}

#define SIMPLE_NOTIFICATOR_ITER typename Callbacks<Function, ListenerContainer>::iterator

TEMPLATE
SIMPLE_NOTIFICATOR_ITER Callbacks<Function, ListenerContainer>::begin() {
  return container_.begin();
}

TEMPLATE
SIMPLE_NOTIFICATOR_ITER Callbacks<Function, ListenerContainer>::end() {
  return container_.end();
}

TEMPLATE
SIMPLE_NOTIFICATOR_ITER Callbacks<Function, ListenerContainer>::erase(SIMPLE_NOTIFICATOR_ITER it) {
  return container_.erase(it);
}

#undef SIMPLE_NOTIFICATOR_ITER

TEMPLATE
template <typename T>
void Callbacks<Function, ListenerContainer>::Invoke(T t1) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    (*it)(t1);
  }
}

TEMPLATE
template <typename T, typename T2>
void Callbacks<Function, ListenerContainer>::Invoke(T t1, T2 t2) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    (*it)(t1, t2);
  }
}

TEMPLATE
template <typename T, typename T2, typename T3>
void Callbacks<Function, ListenerContainer>::Invoke(T t1, T2 t2, T3 t3) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    (*it)(t1, t2, t3);
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4>
void Callbacks<Function, ListenerContainer>::Invoke(T t1, T2 t2, T3 t3, T4 t4) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    (*it)(t1, t2, t3, t4);
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4, typename T5>
void Callbacks<Function, ListenerContainer>::Invoke(T t1, T2 t2, T3 t3, T4 t4, T5 t5) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    (*it)(t1, t2, t3, t4, t5);
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4, typename T5, typename T6>
void Callbacks<Function, ListenerContainer>::Invoke(T t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    (*it)(t1, t2, t3, t4, t5, t6);
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void Callbacks<Function, ListenerContainer>::Invoke(T t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    (*it)(t1, t2, t3, t4, t5, t6, t7);
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
void Callbacks<Function, ListenerContainer>::Invoke(T t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    (*it)(t1, t2, t3, t4, t5, t6, t7, t8);
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
void Callbacks<Function, ListenerContainer>::Invoke(T t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    (*it)(t1, t2, t3, t4, t5, t6, t7, t8, t9);
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
void Callbacks<Function, ListenerContainer>::Invoke(T t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    (*it)(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
  }
}


TEMPLATE
template <typename T>
void Callbacks<Function, ListenerContainer>::InvokeAsync(T t1) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    thread th(bind((*it), t1));
    th.detach();
  }
}

TEMPLATE
template <typename T, typename T2>
void Callbacks<Function, ListenerContainer>::InvokeAsync(T t1, T2 t2) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    thread th(bind((*it), t1, t2));
    th.detach();
  }
}

TEMPLATE
template <typename T, typename T2, typename T3>
void Callbacks<Function, ListenerContainer>::InvokeAsync(T t1, T2 t2, T3 t3) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    thread th(bind((*it), t1, t2, t3));
    th.detach();
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4>
void Callbacks<Function, ListenerContainer>::InvokeAsync(T t1, T2 t2, T3 t3, T4 t4) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    thread th(bind((*it), t1, t2, t3, t4));
    th.detach();
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4, typename T5>
void Callbacks<Function, ListenerContainer>::InvokeAsync(T t1, T2 t2, T3 t3, T4 t4, T5 t5) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    thread th(bind((*it), t1, t2, t3, t4, t5));
    th.detach();
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4, typename T5, typename T6>
void Callbacks<Function, ListenerContainer>::InvokeAsync(T t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    thread th(bind((*it), t1, t2, t3, t4, t5, t6));
    th.detach();
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void Callbacks<Function, ListenerContainer>::InvokeAsync(T t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    thread th(bind((*it), t1, t2, t3, t4, t5, t6, t7));
    th.detach();
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
void Callbacks<Function, ListenerContainer>::InvokeAsync(T t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    thread th(bind((*it), t1, t2, t3, t4, t5, t6, t7, t8));
    th.detach();
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
void Callbacks<Function, ListenerContainer>::InvokeAsync(T t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    thread th(bind((*it), t1, t2, t3, t4, t5, t6, t7, t8, t9));
    th.detach();
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
void Callbacks<Function, ListenerContainer>::InvokeAsync(T t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    thread th(bind((*it), t1, t2, t3, t4, t5, t6, t7, t8, t9, t10));
    th.detach();
  }
}

TEMPLATE
template <typename T>
void Callbacks<Function, ListenerContainer>::operator()(T t1, bool async) {
  if (async) {
    NotifyAsync(t1);
  } else {
    Notify(t1);
  }
}

TEMPLATE
template <typename T, typename T2>
void Callbacks<Function, ListenerContainer>::operator()(T t1, T2 t2, bool async) {
  if (async) {
    NotifyAsync(t1, t2);
  } else {
    Notify(t1, t2);
  }
}

TEMPLATE
template <typename T, typename T2, typename T3>
void Callbacks<Function, ListenerContainer>::operator()(T t1, T2 t2, T3 t3, bool async) {
  if (async) {
    NotifyAsync(t1, t2, t3);
  } else {
    Notify(t1, t2, t3);
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4>
void Callbacks<Function, ListenerContainer>::operator()(T t1, T2 t2, T3 t3, T4 t4, bool async) {
  if (async) {
    NotifyAsync(t1, t2, t3, t4);
  } else {
    Notify(t1, t2, t3, t4);
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4, typename T5>
void Callbacks<Function, ListenerContainer>::operator()(T t1, T2 t2, T3 t3, T4 t4, T5 t5, bool async) {
  if (async) {
    NotifyAsync(t1, t2, t3, t4, t5);
  } else {
    Notify(t1, t2, t3, t4, t5);
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4, typename T5, typename T6>
void Callbacks<Function, ListenerContainer>::operator()(T t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, bool async) {
  if (async) {
    NotifyAsync(t1, t2, t3, t4, t5, t6);
  } else {
    Notify(t1, t2, t3, t4, t5, t6);
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
void Callbacks<Function, ListenerContainer>::operator()(T t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, bool async) {
  if (async) {
    NotifyAsync(t1, t2, t3, t4, t5, t6, t7);
  } else {
    Notify(t1, t2, t3, t4, t5, t6, t7);
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
void Callbacks<Function, ListenerContainer>::operator()(T t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, bool async) {
  if (async) {
    NotifyAsync(t1, t2, t3, t4, t5, t6, t7, t8);
  } else {
    Notify(t1, t2, t3, t4, t5, t6, t7, t8);
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
void Callbacks<Function, ListenerContainer>::operator()(T t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, bool async) {
  if (async) {
    NotifyAsync(t1, t2, t3, t4, t5, t6, t7, t8, t9);
  } else {
    Notify(t1, t2, t3, t4, t5, t6, t7, t8, t9);
  }
}

TEMPLATE
template <typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
void Callbacks<Function, ListenerContainer>::operator()(T t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7, T8 t8, T9 t9, T10 t10, bool async) {
  if (async) {
    NotifyAsync(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
  } else {
    Notify(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10);
  }
}

TEMPLATE
void Callbacks<Function, ListenerContainer>::swap(Callbacks<Function, ListenerContainer>& notificator) {
  container_.swap(notificator.container_);
}

#undef TEMPLATE

}

#endif
