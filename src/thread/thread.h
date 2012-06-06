#ifndef INCLUDE_THREAD_H_
#define INCLUDE_THREAD_H_
#ifdef PLATFORM_POSIX
#include <thread/thread-posix.h>
#elif defined PLATFORM_WIN32
#include <thread/thread-win32.h>
#endif
#endif

#include <thread.h>

namespace os {

Thread::Thread()
    : thread_(new ThreadPlt){}
Thread::~Thread() {delete thread_;}

Mutex::Mutex()
    : mutex_(new MutexPlt){}

Mutex::~Mutex() {
  delete mutex_;
}

ScopedLock::ScopedLock(Mutex& mutex)
    : locker_(new ScopedLockPlt(mutex.mutex_)){}

ScopedLock::~ScopedLock() {delete locker_;}

void ScopedLock::Lock() {
  locker_->Lock();
}

void ScopedLock::Unlock() {
  locker_->Unlock();
}

Semaphore::Semaphore(int count)
    : sem_(new SemaphorePlt(count)){}

Semaphore::~Semaphore() {
  delete sem_;
}

void Semaphore::Wait() {sem_->Wati();}
void Semaphore::Wait(int timeout) {sem_->Wait(timeout);}
void Semaphore::Post() {sem_->Post();}

ThreadLocalStorageKey::ThreadLocalStorageKey()
    : tls_key_(new ThreadLocalStorageKeyPlt){}
ThreadLocalStorageKey::~ThreadLocalStorageKey() {delete tls_key_;}

void* ThreadLocalStorage::Get(ThreadLocalStorageKey* key) {return ThreadLocalStoragePlt::Get(key->tls_key_);}
void ThreadLocalStorage::Set(ThreadLocalStorageKey* key, void* ptr) {ThreadLocalStoragePlt::Set(key->tls_key_, ptr);}

}
