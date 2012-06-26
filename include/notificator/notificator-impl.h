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
#include "../logging.h"
#include "../lib/ref.h"
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
  typedef ListenerAdapter<Listener,Event> Adapter;
  ListenerHandle adapter = make_shared<Adapter>(listener);
  listeners_.insert(ListenerSet(key, adapter));
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
    it.second->Invoke(e);
  }
}

TEMPLATE
inline void Notificator<Event>::NotifyAllAsync(Event e) {
  DEBUG_LOG(Info, "Notificator::NotifyAllAsync Called");
  forEach(ListenerSet& it, listeners_) {
    thread th(bind(&ListenerAdapterBase<Event>::Invoke, it.second.get(), e));
    th.detach();
  }
}

TEMPLATE
inline void Notificator<Event>::NotifyForKey(const char* key, Event e) {
  DEBUG_LOG(Info, "Notificator::NotifyForKey[%s]", key);
  ListenersRange listener_range = listeners_.equal_range(key);
  //Call all liteners that identified by same key.
  forEach(typename Listeners::value_type& it, listener_range) {
    it.second->Invoke(e);
  }
}

TEMPLATE
inline void Notificator<Event>::NotifyForKeyAsync(const char* key, Event e) {
  DEBUG_LOG(Info, "Notificator::NotifyForKeyAsync[%s] Called", key);
  ListenersRange listener_range = listeners_.equal_range(key);
  //Call all liteners that identified by same key.
  forEach(typename Listeners::value_type& it, listener_range) {
    thread th(bind(&ListenerAdapterBase<Event>::Invoke, it.second.get(), e));
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
#define TEMPLATE template <typename Event, typename ListenerContainer>

TEMPLATE
SimpleNotificator<Event, ListenerContainer>::SimpleNotificator(){}

TEMPLATE
SimpleNotificator<Event, ListenerContainer>::~SimpleNotificator(){}

TEMPLATE
template <typename Listener>
void SimpleNotificator<Event, ListenerContainer>::AddListener(Listener listener) {
  typedef ListenerAdapter<Listener,Event> Adapter;
  ListenerHandle handle = make_shared<Adapter>(listener);
  container_.push_back(handle);
}

TEMPLATE
template <typename Listener>
void SimpleNotificator<Event, ListenerContainer>::operator += (Listener listener) {
  AddListener(listener);
}

TEMPLATE
void SimpleNotificator<Event, ListenerContainer>::RemoveListener() {
  container_.clear();
}

#define SIMPLE_NOTIFICATOR_ITER typename SimpleNotificator<Event, ListenerContainer>::iterator

TEMPLATE
SIMPLE_NOTIFICATOR_ITER SimpleNotificator<Event, ListenerContainer>::begin() {
  return container_.begin();
}

TEMPLATE
SIMPLE_NOTIFICATOR_ITER SimpleNotificator<Event, ListenerContainer>::end() {
  return container_.end();
}

TEMPLATE
SIMPLE_NOTIFICATOR_ITER SimpleNotificator<Event, ListenerContainer>::erase(SIMPLE_NOTIFICATOR_ITER it) {
  return container_.erase(it);
}

#undef SIMPLE_NOTIFICATOR_ITER

TEMPLATE
void SimpleNotificator<Event, ListenerContainer>::Notify(Event e) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    it->Invoke(e);
  }
}

TEMPLATE
void SimpleNotificator<Event, ListenerContainer>::NotifyAsync(Event e) {
  forEach(typename ListenerContainer::iterator::value_type& it, container_) {
    thread th(bind(&ListenerAdapterBase<Event>::Invoke, (*it), e));
    th.detach();
  }
}

TEMPLATE
void SimpleNotificator<Event, ListenerContainer>::operator()(Event e, bool async) {
  if (async) {
    NotifyAsync(e);
  } else {
    Notify(e);
  }
}

TEMPLATE
void SimpleNotificator<Event, ListenerContainer>::swap(SimpleNotificator<Event, ListenerContainer>& notificator) {
  container_.swap(notificator.container_);
}

#undef TEMPLATE

}

#endif
