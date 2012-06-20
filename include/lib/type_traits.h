#ifndef INCLUDE_TYPE_TRAITS_H_
#define INCLUDE_TYPE_TRAITS_H_

#if defined(HAVE_TYPE_TRAITS)
namespace os {
using std::is_pointer;
using std::is_reference;
using std::remove_pointer;
using std::remove_reference;
using std::add_pointer;
using std::add_reference;
}
#elif defined(HAVE_BOOST_TYPE_TRAITS_HPP)
namespace os {
using boost::is_pointer;
using boost::is_reference;
using boost::remove_pointer;
using boost::remove_reference;
using boost::add_pointer;
using boost::add_reference;
}
#endif

#endif
