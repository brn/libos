#ifndef LIB_UNORDERED_MAP_H_
#define LIB_UNORDERED_MAP_H_
#ifdef HAVE_UNORDERED_MAP_H
#include <unordered_map>
namespace os {
namespace roastlib {
using std::unordered_map;
using std::unordered_multimap;
}
}
#else
#include <boost/unordered_map.hpp>
namespace os {
namespace roastlib {
using boost::unordered_map;
using boost::unordered_multimap;
}
}

#endif
#endif
