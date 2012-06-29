#ifndef INCLUDE_LIB_ATOMIC_H_
#define INCLUDE_LIB_ATOMIC_H_
#include "../libos_config.h"
#if defined(HAVE_BOOST_DETAIL_ATOMIC_COUNT_HPP)
#include <boost/detail/atomic_count.hpp>
namespace os {
using boost::detail::atomic_count;
}
#endif

#endif
