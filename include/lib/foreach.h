#ifndef INCLUDE_LIB_FOREACH_H_
#define INCLUDE_LIB_FOREACH_H_
#if defined(HAVE_BOOST_FOREACH_HPP)
#include <boost/foreach.hpp>
#define forEach(var, cont) BOOST_FOREACH(var, cont)
#define rForEach(var, cont) BOOST_REVERSE_FOREACH(var, cont)
#endif
#endif
