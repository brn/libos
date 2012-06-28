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
#ifndef INCLUDE_CALLBACKS_H_
#define INCLUDE_CALLBACKS_H_
#include <vector>
#include "../lib/unordered_map.h"
#include "../lib/shared_ptr.h"
#include "../lib/function.h"
#include "../lib/type_traits.h"
#include "../lib/pp.h"
namespace os {

#ifndef PARAMETER_LENGTH
#define PARAMETER_LENGTH 11
#endif

template <typename Function, typename ListenerContainer = std::vector<shared_ptr<function<Function> > > >
class Callbacks {
 public :
  typedef function<Function> EventListener;
  typedef shared_ptr<EventListener> ListenerHandle;
  typedef typename ListenerContainer::iterator iterator;
  typedef Function result_type;
  typedef ListenerContainer container_type;
  Callbacks();
  ~Callbacks();
  template <typename Listener>
  void Add(Listener listener);
  template <typename Listener>
  void operator += (Listener listener);
  void Remove();
  iterator begin();
  iterator end();
  iterator erase(iterator);
  
#define TEMPLATE_ARGUMENTS(z, n, name)                                \
  template<BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), typename T)>    \
  void name(BOOST_PP_ENUM_BINARY_PARAMS_Z(z, BOOST_PP_INC(n), T, t));

  BOOST_PP_REPEAT(PARAMETER_LENGTH, TEMPLATE_ARGUMENTS, Invoke)
  BOOST_PP_REPEAT(PARAMETER_LENGTH, TEMPLATE_ARGUMENTS, InvokeAsync)

#undef TEMPLATE_ARGUMENTS

#define TEMPLATE_ARGUMENTS(z, n, name)                                \
  template<BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), typename T)>    \
  void operator()(BOOST_PP_ENUM_BINARY_PARAMS_Z(z, BOOST_PP_INC(n), T, t), bool async = false);

  BOOST_PP_REPEAT(PARAMETER_LENGTH, TEMPLATE_ARGUMENTS, nil)

#undef TEMPLATE_ARGUMENTS
  
  int size() const {return container_.size();}
  void swap(Callbacks<Function, ListenerContainer>& notificator);
  bool empty() const {return container_.empty();}
 private :
  ListenerContainer container_;
};

}

#include "callbacks-impl.h"

#endif
