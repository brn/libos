#ifndef INCLUDE_TYPE_TRAITS_H_
#define INCLUDE_TYPE_TRAITS_H_
#include "../libos_config.h"
#if defined(HAVE_TYPE_TRAITS)
#include <type_traits>
namespace os {
using std::is_pointer;
using std::is_reference;
using std::remove_pointer;
using std::remove_reference;
using std::add_pointer;
using std::add_reference;
using std::is_convertible;
using std::is_class;
}
#elif defined(HAVE_BOOST_TYPE_TRAITS_HPP)
#include <boost/type_traits.hpp>
namespace os {
using boost::is_pointer;
using boost::is_reference;
using boost::remove_pointer;
using boost::remove_reference;
using boost::add_pointer;
using boost::add_reference;
using boost::is_convertible;
using boost::is_class;
}
#endif

#endif
