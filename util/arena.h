#ifndef LEVELDB_UTIL_ARENA_H_
#define LEVELDB_UTIL_ARENA_H_

#include <vector>
#include <stdint.h>
#include <cstddef>

namespace leveldb {

class Arena {
public: 
    Arena();
    ~Arena();

    // allocate a memory block with size_ bytes
    char* Allocate(size_t size_);

    // Allocate memory with the normal alignment guarantees provided by malloc
    char* AllocateAligned(size_t size_);

    // Returns an estimate of the total memory usage of data allocated by the arena.
    size_t MemoryUsage() const {
        return blocks_memory + blocks_arr.capacity() * sizeof(char*);
    }

private: 
    char* AllocateNewBlock(size_t block_size);
    char* Reallocate(size_t size_);

    char* alloc_ptr;    // the first addr of the remaining alloc bytes
    size_t alloc_bytes_remaining;

    // Array of new[] allocated memory blocks
    std::vector<char*> blocks_arr;

    size_t blocks_memory;

    Arena(const Arena&);
    void operator=(const Arena&);
};

inline char* Arena::Allocate(size_t size_) {
    if(size_ <= alloc_bytes_remaining) {
        // if there is left space in block to alloc
        char* ret = alloc_ptr;
        alloc_ptr += size_;
        alloc_bytes_remaining -= size_;
        return ret;
    }
    
    return Reallocate(size_);
}

}


#endif