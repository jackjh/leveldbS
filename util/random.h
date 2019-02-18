#ifndef LEVELDB_UTIL_RANDOM_H_
#define LEVELDB_UTIL_RANDOM_H_

#include <stdint.h>

namespace leveldb {

class Random {
private: 
    uint32_t seed;

public: 
    explicit Random(uint32_t s) : seed(s & 0x7fffffffU) {
        if(seed == 0 || seed == 0x7fffffffL) {
            seed = 1;
        }
    }

    uint32_t Next() {
        static uint32_t M = 2147483647L;   // (2^31 - 1)
        static uint32_t A = 16807;

        /*
        ** seed = (seed * A) % M;
        ** H ==> high 33 bit, L ==> low 31 bit
        ** product = (H << 31) + L = (H * 2^31) + L, product % M = (H * M + H + L) % M = H + L;
        ** H = product >> 31, L = product & M;
        */
        uint64_t product = seed * A;
        seed = static_cast<uint32_t>((product >> 31) + (product & M));

        if(seed > M) {
            seed -= M;
        }
        
        return seed;
    }

    uint32_t UniformRam(int n) {
        return Next() % n;
    }

    bool OneIn(int n) {
        return (UniformRam(n) == 0);
    }
};

}


#endif