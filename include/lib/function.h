#ifndef INCLUDE_LIB_FUNCTION_H_
#define INCLUDE_LIB_FUNCTION_H_
#include "../libos_config.h"
#if defined(HAVE_STD_FUNCTION)
#include <functional>
namespace os {
using std::function;
}
#elif defined(HAVE_BOOST_FUNCTION)
#include <boost/function.hpp>
namespace os {
using boost::function;
}
#endif

#endif
