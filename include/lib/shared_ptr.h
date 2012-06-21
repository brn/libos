#ifndef INCLUDE_LIB_SHARED_PTR_H_
#define INCLUDE_LIB_SHARED_PTR_H_

#if defined(HAVE_SHARED_PTR)
#include <memory>
namespace os {
using std::shared_ptr;
}
#elif defined(HAVE_BOOST_SHARED_PTR)
#include <boost/shared_ptr.hpp>
namespace os {
using boost::shared_ptr;
}
#endif

#endif
