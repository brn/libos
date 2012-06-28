#ifndef INCLUDE_LIB_BIND_H_
#define INCLUDE_LIB_BIND_H_
#include "../libos_config.h"
#if defined(HAVE_STD_BIND)
#include <functional>
namespace os {
namespace placeholders = std::placeholders;
using std::bind;
}
#elif defined(HAVE_BOOST_BIND)
#include <boost/bind.hpp>
namespace os {
using boost::bind;
namespace placeholders {
static boost::arg<1> _1;
static boost::arg<2> _2;
static boost::arg<3> _3;
static boost::arg<4> _4;
static boost::arg<5> _5;
static boost::arg<6> _6;
static boost::arg<7> _7;
static boost::arg<8> _8;
static boost::arg<9> _9;
}
}
#endif
#endif
