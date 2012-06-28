#ifndef INCLUDE_LIB_FOREACH_H_
#define INCLUDE_LIB_FOREACH_H_
#include "../libos_config.h"
#if defined(HAVE_BOOST_FOREACH_HPP)
#include <boost/foreach.hpp>
#define foreach(var, cont) BOOST_FOREACH(var, cont)
#define rforeach(var, cont) BOOST_REVERSE_FOREACH(var, cont)
#endif
#endif
