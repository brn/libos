#ifndef INCLUDE_LIB_REF_H_
#define INCLUDE_LIB_REF_H_
#include "../libos_config.h"
#if defined(HAVE_STD_REF)
#include <functional>
namespace os {
using std::ref;
}
#elif defined(HAVE_BOOST_REF)
#include <boost/ref.hpp>
namespace os {
using boost::ref;
}
#endif
#endif
