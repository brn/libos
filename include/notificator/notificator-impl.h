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
#include "../lib/foreach.h"
#include "../thread.h"
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
  //Object lifetime is controlled by Notificator::pool_,
  //so the notificator instance that create ListenerAdapter is destroyed,
  //ListenerAdapter is destroyed too.
  ListenerAdapter<Listener,Event>* adapter = new(&pool_) ListenerAdapter<Listener,Event>(listener);
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
  //std::for_each(listeners_.begin(), listeners_.end(), Invocator_<Event>(e));
  forEach(Listeners it, listeners_) {
    (*it)->Invoke(e);
  }
}

TEMPLATE
inline void Notificator<Event>::NotifyAllAsync(Event e) {
  thread t(bind(&Notificator<Event>::NotifyAll, this), key, e);
  t.detach();
}

TEMPLATE
inline void Notificator<Event>::NotifyForKey(const char* key, Event e) {
  DEBUG_LOG(Info, "Notificator::NotifyForKey[%s]", key);
  ListenersRange listener_range = listeners_.equal_range(key);
  //Call all liteners that identified by same key.
  forEach(Listeners::value_type& it, listener_range) {
    it.second->Invoke(e);
  }
}

TEMPLATE
inline void Notificator<Event>::NotifyForKeyAsync(const char* key, Event e) {
  thread t(bind(&Notificator<Event>::NotifyForKey, this), key, e);
  t.detach();
}


#undef TEMPLATE

}

#endif
