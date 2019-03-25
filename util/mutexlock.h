#ifndef LEVELDB_UTIL_MUTEXLOCK_H_
#define LEVELDB_UTIL_MUTEXLOCK_H_

#include "port/port_stdcxx.h"

namespace leveldb {

class MutexLock {
public: 
    explicit MutexLock(port::Mutex* mu_) : mu(mu_) {
        this->mu->Lock();
    }

    ~MutexLock() {
        this->mu->Unlock();
    }

    MutexLock(const MutexLock&) = delete;
    MutexLock& operator=(const MutexLock&) = delete;

private: 
    port::Mutex* const mu;
};

}


#endif