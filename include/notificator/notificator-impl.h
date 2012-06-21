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
namespace os {

#define TEMPLATE template<typename Event>

TEMPLATE
inline Notificator<Event>::Notificator(){}

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
inline void Notificator<Event>::RemoveListener(const char* key) {
  ListenersIterator it = listeners_.find(key);
  if (it != listeners_.end()) {
    listeners_.erase(it);
  }
}


TEMPLATE
inline void Notificator<Event>::NotifyAll(Event e) {
  DEBUG_LOG(Info, "Notificator::NotifyAll Called");
  forEach(ListenerSet& it, listeners_) {
    it.second->Invoke(e);
  }
}

TEMPLATE
inline void Notificator<Event>::NotifyAllAsync(Event e) {
  DEBUG_LOG(Info, "Notificator::NotifyAllAsync Called");
  NotifyAsync(bind(&Notificator<Event>::NotifyAll, this, e));
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
  NotifyAsync(bind(&Notificator<Event>::NotifyForKey, this, key, e));
}

TEMPLATE
template <typename T>
inline void Notificator<Event>::NotifyAsync(T t) {
  thread th(t);
  th.detach();
}

#undef TEMPLATE

}

#endif
