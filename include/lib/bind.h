#ifndef INCLUDE_LIB_BIND_H_
#define INCLUDE_LIB_BIND_H_
#include "../libos_config.h"
#if defined(HAVE_STD_BIND)
#include <functional>
namespace os {
using std::bind;
}
#elif defined(HAVE_BOOST_BIND)
#include <boost/bind.hpp>
namespace os {
using boost::bind;
}
#endif
#endif
