#ifndef INCLUDE_LIB_UNIQUE_PTR_H_
#define INCLUDE_LIB_UNIQUE_PTR_H_

#if defined(HAVE_UNIQUE_PTR)
#include <memory>
namespace os {
template<typename T, typename Deleter>
struct unique_ptr {
  typedef std::unique_ptr<T, Deleter> type;
};
template<typename T>
struct unique_ptr {
  typedef std::unique_ptr<T> type;
};
#elif defined(HAVE_BOOST_UNIQUE_PTR)
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#include <boost/checked_delete.hpp>
namespace os {
template<typename T, typename Deleter = boost::checked_deleter<T> >
struct unique_ptr {
  typedef boost::interprocess::unique_ptr<T, Deleter> type;
};
}
#endif

#endif
