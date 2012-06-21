#ifndef INCLUDE_THREAD_H_
#define INCLUDE_THREAD_H_
#include "libos_config.h"
#if defined(HAVE_THREAD)
#include <thread>
namespace os {
using std::thread::thread;
namespace this_thread = std::this_thread;
}
#elif defined(HAVE_BOOST_THREAD_HPP)
#include <boost/thread.hpp>
namespace os {
using boost::thread;
namespace this_thread = boost::this_thread;
}
#elif defined(HAVE_TR1_THREAD)
#include <tr1/thread>
namespace os {
using std::tr1::thread;
}
#endif

#if defined(HAVE_STD_MUTEX)
#include <mutex>
namespace os {
using std::mutex;
}
#elif defined(HAVE_BOOST_MUTEX)
namespace os {
using boost::mutex;
}
#endif

#if defined(HAVE_STD_UNIQUE_LOCK)
namespace os {
using std::unique_lock;
}
#elif defined(HAVE_BOOST_UNIQUE_LOCK)
namespace os {
using boost::unique_lock;
}
#endif

#if defined(HAVE_STD_LOCK_GUARD)
namespace os {
using std::lock_guard;
}
#elif defined(HAVE_BOOST_LOCK_GUARD)
namespace os {
using boost::lock_guard;
}
#endif

#if defined(HAVE_STD_RECURSIVE_MUTEX)
namespace os {
using std::recursive_mutex;
}
#elif defined(HAVE_BOOST_RECURSIVE_MUTEX)
namespace os {
using boost::recursive_mutex;
}
#endif

#if defined(HAVE_STD_TIMED_MUTEX)
namespace os {
using std::timed_mutex;
}
#elif defined(HAVE_BOOST_TIMED_MUTEX)
namespace os {
using boost::timed_mutex;
}
#endif

#if defined(HAVE_BOOST_THREAD_SPECIFIC_PTR) && defined(HAVE_BOOST_THREAD_TSS_HPP)
#include <boost/thread/tss.hpp>
namespace os {
using boost::thread_specific_ptr;
}
#endif

#endif
