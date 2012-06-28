#ifndef INCLUDE_UTILITIES_H_
#define INCLUDE_UTILITIES_H_
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <errno.h>
#include "static_assert.h"
#include "lib/unordered_map.h"

#if defined DEBUG
#if defined __GNUC__

#define ASSERT(expect, result)                                          \
  if ((expect) != (result)) {os::FPrintf(stderr, "assertion failed -> %s == %s\n in file %s at line %d\n in function %s\n", #result, #expect, __FILE__, __LINE__, __PRETTY_FUNCTION__);abort();}
#define FATAL(msg) os::FPrintf(stderr, "Fatal error occured, so process no longer exist.\nin file %s at line %d\n in function %s\n%s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, msg);abort();

#elif defined __func__

#define ASSERT(expect, result) if ((expect) != (result)){os::FPrintf(stderr, "assertion failed -> %s == %s\n in file %s at line %d\n in function %s\n", #result, #expect, __FILE__, __LINE__, __func__);abort();}
#define FATAL(msg) os::FPrintf(stderr, "Fatal error occured, so process no longer exist.\nin file %s at line %d\n in function %s\n%s\n", __FILE__, __LINE__, __func__, msg);abort();

#elif defined __FUNC__

#define ASSERT(expect, result) if ((expect) != (result)){os::FPrintf(stderr, "assertion failed -> %s == %s\n in file %s at line %d\n in function \n", #result, #expect, __FILE__, __LINE__, __FUNC__);abort();}
#define FATAL(msg) os::FPrintf(stderr, "Fatal error occured, so process no longer exist.\nin file %s at line %d\n in function %s\n%s\n", __FILE__, __LINE__, __FUNC__, msg);abort();

#else
#define ASSERT(expect, result) if ((expect) != (result)){os::FPrintf(stderr, "assertion failed -> %s == %s\n in file %s at line %d\n", #result, #expect, __FILE__, __LINE__);abort();}
#define FATAL(msg) os::FPrintf(stderr, "Fatal error occured, so process no longer exist.\nin file %s at line %d\n%s\n", __FILE__, __LINE__, msg);abort();
#endif

#elif defined NDEBUG
#define ASSERT(expect, result)
#define FATAL(msg)
#else
#define ASSERT(expect, result)
#define FATAL(msg)
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1600) && !defined(__MINGW32__)

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;  // NOLINT
typedef unsigned short uint16_t;  // NOLINT
typedef int int32_t;
typedef unsigned int uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

#define INT8_MAX CHAR_MAX
#define INT8_MIN CHAR_MIN
#define UINT8_MAX UCHAR_MAX
#define INT16_MAX SHRT_MAX
#define INT16_MIN SHRT_MIN
#define UINT16_MAX SHORT_MAX
#define INT32_MAX INT_MAX
#define INT32_MIN INT_MIN
#define UINT32_MAX UINT_MAX
#define INT64_MAX _I64_MAX
#define INT64_MIN _I64_MIN
#define UINT64_MAX _UI64_MAX

// intptr_t and friends are defined in crtdefs.h through stdio.h.
#include <crtdefs.h>
#else

#include <stdint.h>

#endif

namespace os {

void Strerror(std::string* buf, int err);
void Printf(const char* format, ...);
void SPrintf(std::string*, const char* format, ...);
void VSPrintf(std::string* buf, const char* format, va_list args);
void VFPrintf(FILE* fp, const char* format, ...);
void FPrintf(FILE* fp, const char* format, ...);
FILE* FOpen(const char* filename, const char* mode);
void FClose(FILE* fp);
void GetEnv(std::string *buf, const char* env);
bool Sleep(int nano_time);
int Utime(const char* path);
time_t Time(time_t* time);
int Asctime(std::string* buf, tm* tm);
int LocalTime(tm* t, time_t* time);
void OnExit(void(*callback)());
void GetLastError(std::string* buf);
FILE* POpen(const char* name, const char* mode);
void PClose(FILE* fp);
char* Strdup(const char* path);

class Static{
  inline Static (){};
  inline Static (const Static&){};
  inline void operator = (const Static&){};
  inline void operator delete [] (void*) {};
};

class UnAllocatable {
 private :
  inline static void* operator new (size_t size);
  inline static void* operator new [] (size_t size);
  inline static void* operator new (size_t size, void*);
  inline static void* operator new [] (size_t size, void*);
};
class UnAllocatableExternal {
 public :
  inline static void operator delete (void* ptr) { ::operator delete(ptr); }
  inline static void operator delete [] (void* ptr) { ::operator delete[](ptr); }
  inline static void operator delete (void* ptr, void* buf) { ::operator delete(ptr, buf); }
  inline static void operator delete [] (void* ptr, void* buf) { ::operator delete[](ptr, buf); }
 protected :
  inline static void* operator new (size_t size) { return ::operator new(size); };
  inline static void* operator new [] (size_t size) { return ::operator new[](size); };
  inline static void* operator new (size_t size, void* buf) { return ::operator new(size, buf); };
  inline static void* operator new [] (size_t size, void* buf)  { return ::operator new[](size, buf); };;
};

class Uncopyable {
 private:
  inline void operator = (const Uncopyable&) {};
  inline Uncopyable (const Uncopyable&) {};
 public:
  inline Uncopyable () {};
};

namespace memory {
class Pool;
/**
 * @class
 * The base of the lifetime managed pointer.
 * To allocate from the pool,
 * must inherit this class as public.
 */
class Allocated {
  friend class Pool;
 public :
  Allocated(){}
  virtual ~Allocated(){}
  //The placement new for the pool allocation.
  void* operator new(size_t size, Pool* pool);
  void operator delete(void* ptr);
  void operator delete(void* ptr, Pool* pool);
 private :
  char allocated_;
  Allocated* prev_;
  Allocated* next_;
};

template <size_t>
class Chunk;

struct Block;

/**
 * @class
 * The pointer lifetime managable allocator.
 * This memory space is allocation only,
 * it can not free an each space, to free memory block,
 * destroy Pool class.
 */
class Pool : private Uncopyable {
  friend class Allocated;
 public :
  Pool();
  ~Pool();
  /**
   * @public
   * free memory space.
   */
  void Free();
  /**
   * @public
   * allocate to tls space.
   */
  static Pool* Local();
  static size_t Align(size_t offset, size_t alignment);
  template <typename T>
  T* Alloc(size_t size = 1);
 private :
  void* AllocateBlock(size_t size);
  ///Default memory block size.
  static const size_t kDefaultSize = 3072;
  /**
   * @private
   * Allocate the memory and add used block list,
   * if the Pool class is destroyed,
   * all allocated memory is destoryed too.
   * Allocatable size is kDefaultSize.
   */
  void* AllocLinkedList(size_t size);
  size_t size;
  Allocated* current_;
  Allocated* head_;
  Chunk<kDefaultSize>* head_chunk_;
  Chunk<kDefaultSize>* current_chunk_;
  Block* head_block_;
  Block* current_block_;
};
}

template <typename T,int bit_bands>
class BitVector {
 public :
  BitVector() : set_(0){}
  ~BitVector(){}
  BitVector(const BitVector& vec) {
    set_ = vec.set_;
  }
  const BitVector<T,bit_bands>& operator = (const BitVector<T,bit_bands>& vec) {
    set_ = vec.set_;
    return (*this);
  }
  void Set(int val) {
    ASSERT(true, (bit_bands >= val && val > -1));
    set_ |= (1 << val);
  }
  void UnSet(int val) {
    ASSERT(true, (bit_bands >= val && val > -1));
    set_ &= (~(1 << val));
  }
  void Reverse(int val) {
    ASSERT(true, (bit_bands >= val && val > -1));
    set_ ^= (1 << val);
  }
  bool At(int val) const {
    ASSERT(true, (bit_bands >= val));
    T tmp = (1 << val);
    return (set_ & tmp) == tmp;
  }
  bool operator[] (int val) const {
    return At(val);
  }
 private :
  STATIC_ASSERT(bit_bands > 0);
  T set_;
};


typedef BitVector<uint8_t,8>  BitVector8;
typedef BitVector<uint16_t,16> BitVector16;
typedef BitVector<uint32_t,32> BitVector32;
typedef BitVector<uint64_t,64> BitVector64;

namespace bits {

inline int BitCount8(uint8_t bit) {
  uint8_t count = (bit & 0x55) + ((bit >> 1) & 0x55);
  count = (count & 0x33) + ((count >> 2) & 0x33);
  return (count & 0x0f) + ((count >> 4) & 0x0f);
}

inline int BitCount16(uint16_t bit) {
  uint16_t count = (bit & 0x5555) + ((bit >> 1) & 0x5555);
  count = (count & 0x3333) + ((count >> 2) & 0x3333);
  count = (count & 0x0f0f) + ((count >> 4) & 0x0f0f);
  return (count & 0x00ff) + ((count >> 8) & 0x00ff);
}

inline int BitCount32(uint32_t bit) {
  uint32_t count = (bit & 0x55555555) + ((bit >> 1) & 0x55555555);
  count = (count & 0x33333333) + ((count >> 2) & 0x33333333);
  count = (count & 0x0f0f0f0f) + ((count >> 4) & 0x0f0f0f0f);
  count = (count & 0x00ff00ff) + ((count >> 8) & 0x00ff00ff);
  return (count & 0x0000ffff) + ((count >> 16) & 0x0000ffff);
}


inline int BitCount64(uint64_t bit) {
  uint64_t count = (bit & 0x5555555555555555ULL) + ((bit >> 1) & 0x5555555555555555ULL);
  count = (count & 0x3333333333333333ULL) + ((count >> 2) & 0x3333333333333333ULL);
  count = (count & 0x0f0f0f0f0f0f0f0fULL) + ((count >> 4) & 0x0f0f0f0f0f0f0f0fULL);
  count = (count & 0x00ff00ff00ff00ffULL) + ((count >> 8) & 0x00ff00ff00ff00ffULL);
  count = (count & 0x0000ffff0000ffffULL) + ((count >> 16) & 0x0000ffff0000ffffULL);
  return static_cast<int>((count & 0x00000000ffffffffULL) + ((count >> 32ULL) & 0x00000000ffffffffULL));
}

inline int MSB8bit(uint8_t bit) {
  if (bit == 0) return 0;
  bit |= (bit >> 1);
  bit |= (bit >> 2);
  bit |= (bit >> 4);
  return BitCount8(bit) - 1;
}

inline int MSB16bit(uint16_t bit) {
  if (bit == 0) return 0;
  bit |= (bit >> 1);
  bit |= (bit >> 2);
  bit |= (bit >> 4);
  bit |= (bit >> 8);
  return BitCount16(bit) - 1;
}

inline int MSB32bit(uint32_t bit) {
  if (bit == 0) return 0;
  bit |= (bit >> 1);
  bit |= (bit >> 2);
  bit |= (bit >> 4);
  bit |= (bit >> 8);
  bit |= (bit >> 16);
  return BitCount32(bit) - 1;
}

inline int MSB64(uint64_t bit) {
  if (bit == 0) return 0;
  bit |= (bit >> 1);
  bit |= (bit >> 2);
  bit |= (bit >> 4);
  bit |= (bit >> 8);
  bit |= (bit >> 16);
  bit |= (bit >> 32ULL);
  return BitCount64(bit) - 1;
}

}
}
#endif
#include "pool-inl.h"

