#ifndef mocha_static_assert_h_
#define mocha_static_assert_h_

template <bool, int> struct StaticAssert{
  typedef int static_assertion_is_failed;
};

template <int n>
struct StaticAssert<false, n> {};

#define JOIN(a, b) JOIN_(a, b)
#define JOIN_(a, b) a##b
#define STATIC_ASSERT(expr)                                             \
    typedef typename StaticAssert<static_cast<bool>((expr)), __LINE__>::static_assertion_is_failed JOIN(Assertion_failed_in_, __LINE__)

#endif
