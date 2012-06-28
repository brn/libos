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
#ifndef UTILS_NOTIFICATOR_H_
#define UTILS_NOTIFICATOR_H_

#include "../utilities.h"
#include "../thread.h"
#include "../worker.h"
#include "../lib/unordered_map.h"
#include "../lib/shared_ptr.h"
#include "../lib/function.h"
#include "../lib/type_traits.h"
#include "../lib/pp.h"

#ifndef PARAMETER_LENGTH
#define PARAMETER_LENGTH 11
#endif

namespace os {
/**
 * @class
 * The event observer implementation.
 */
template<typename Signature>
class Notificator {
  typedef function<Signature> ListenerFunction;
  typedef shared_ptr<ListenerFunction> ListenerHandle;
  //The unordered_multimap entry type.
  typedef std::pair<const char*, ListenerHandle> ListenerSet;
  typedef unordered_multimap<std::string, ListenerHandle> Listeners;
  typedef typename Listeners::iterator ListenersIterator;
  //The unordered_multimap equal_range type.
  typedef std::pair<ListenersIterator, ListenersIterator> ListenersRange;
  
 public :
  Notificator();
  virtual ~Notificator(){};
  Notificator(const Notificator<Signature>& notificator);
  const Notificator<Signature>& operator = (const Notificator<Signature>& notificator);
  /**
   * @public
   * @param {const char*} key
   * @param {Listener} listener
   * Add listener to the observer.
   * The added listener is identified by the string key.
   */
  template <typename Listener>
  void AddListener(const char* key, Listener listener);

  void RemoveListener(const char* key);

  template <typename Listener>
  void operator += (std::pair<const char*, Listener> listener_pack);

  void operator -= (const char* key);

#define TEMPLATE_ARGUMENTS(z, n, name)                                  \
  template<BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), typename T)>       \
  void name(BOOST_PP_ENUM_BINARY_PARAMS_Z(z, BOOST_PP_INC(n), T, t));
  
  
  /**
   * @public
   * @param {Event} e
   * Notify event to the all listeners.
   */
  BOOST_PP_REPEAT(PARAMETER_LENGTH, TEMPLATE_ARGUMENTS, NotifyAll);
  BOOST_PP_REPEAT(PARAMETER_LENGTH, TEMPLATE_ARGUMENTS, NotifyAllAsync);
  
#undef TEMPLATE_ARGUMENTS
  
  /**
   * @public
   * @param {const char*} key
   * @param {Event} e
   * Notify event to the observer that is identified by key.
   */
#define TEMPLATE_ARGUMENTS(z, n, _)                                  \
  template<BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), typename T)>       \
  void _(const char* key, BOOST_PP_ENUM_BINARY_PARAMS_Z(z, BOOST_PP_INC(n), T, t));


  BOOST_PP_REPEAT(PARAMETER_LENGTH, TEMPLATE_ARGUMENTS, NotifyForKey)
  BOOST_PP_REPEAT(PARAMETER_LENGTH, TEMPLATE_ARGUMENTS, NotifyForKeyAsync)

#undef TEMPLATE_ARGUMENTS

#define OPERATOR_CALL_ARGUMENTS(z, n, _)                               \
  template<BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), typename T)>      \
  void operator()(BOOST_PP_ENUM_BINARY_PARAMS_Z(z, BOOST_PP_INC(n), T, t), bool async = false);
  BOOST_PP_REPEAT(PARAMETER_LENGTH, OPERATOR_CALL_ARGUMENTS, nil)
#undef OPERATOR_CALL_ARGUMENTS
  
#define OPERATOR_CALL_FOR_KEY_ARGUMENTS(z, n, _)                               \
  template<BOOST_PP_ENUM_PARAMS_Z(z, BOOST_PP_INC(n), typename T)>      \
  void operator()(const char* key, BOOST_PP_ENUM_BINARY_PARAMS_Z(z, BOOST_PP_INC(n), T, t), bool async = false);
  BOOST_PP_REPEAT(PARAMETER_LENGTH, OPERATOR_CALL_FOR_KEY_ARGUMENTS, nil)
#undef OPERATOR_CALL_FOR_KEY_ARGUMENTS
  
  /**
   * @public
   * @returns {int}
   * Return registered listener number.
   */
  int size() const {return listeners_.size();}
  void swap(Notificator<Signature>& notificator);
  bool empty() const {return listeners_.empty();}
 private :
  Worker workers_;
  Listeners listeners_;
};

}
#include "notificator-impl.h"

#endif
