#ifndef UTILS_LISTENER_ADAPTER_IMPL_H_
#define UTILS_LISTENER_ADAPTER_IMPL_H_
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
#include "../type_convertor.h"
namespace os {

template <typename Listener, typename Event>
inline ListenerAdapter<Listener,Event>::ListenerAdapter(Listener listener)
    : ListenerAdapterBase<Event>(),
      listener_(listener){}

template <typename Listener, typename Event>
inline void ListenerAdapter<Listener,Event>::Invoke(Event e) {
  TypeConvertor<Listener>::ToRef(listener_)(e);
}

template <typename Listener, typename Event>
inline ListenerAdapter<Listener,Event>::ListenerAdapter(const ListenerAdapter& adapter) {
  listener_ = adapter.listener_;
}
}

#endif
