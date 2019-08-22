#ifndef LEVELDB_PORT_PORT_STDCXX_H_
#define LEVELDB_PORT_PORT_STDCXX_H_

#include <stddef.h>
#include <stdint.h>
#include <mutex>
#include <condition_variable>
#include <cassert>

namespace leveldb {

namespace port {

class CondVar;

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
    friend class CondVar;
    std::mutex mu;
};

class CondVar {
public: 
    explicit CondVar(Mutex* m) : mu(m) {
        assert(mu != nullptr);
    }

    ~CondVar() = default;
    
    CondVar(const CondVar&) = delete;
    CondVar& operator=(const CondVar&) = delete;

    void Wait() {
        std::unique_lock<std::mutex> lock(mu->mu, std::adopt_lock);
        cv.wait(lock);
        lock.release();
    }

    void Signal() {
        cv.notify_one();
    }

    void SignalAll() {
        cv.notify_all();
    }

private: 
    std::condition_variable cv;
    Mutex* const mu;
};

}

}


#endif