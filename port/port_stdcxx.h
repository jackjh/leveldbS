#ifndef LEVELDB_PORT_PORT_STDCXX_H_
#define LEVELDB_PORT_PORT_STDCXX_H_

#include <stddef.h>
#include <stdint.h>
#include <mutex>
#include <condition_variable>

namespace leveldb {

namespace port {

class Mutex {
public: 
    Mutex() = default;
    ~Mutex() = default;

    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;

    void Lock() {
        mu.lock();
    }

    void Unlock() {
        mu.unlock();
    }

private:
    std::mutex mu;
};

}

}


#endif