#ifndef INCLUDE_LIB_MAKE_SHARED_H_
#define INCLUDE_LIB_MAKE_SHARED_H_

#if defined(HAVE_STD_MAKE_SHARED)
#include <memory>
namespace os {
using std::make_shared;
}
#elif defined(HAVE_BOOST_MAKE_SHARED)
#include <boost/make_shared.hpp>
namespace os {
using boost::make_shared;
}
#endif
#endif
