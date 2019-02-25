#include <assert.h>
#include "util/arena.h"

namespace leveldb {

static const int kBlockSize = 4096;

Arena::Arena() {
    blocks_memory = 0;
    alloc_ptr = NULL;
    alloc_bytes_remaining  = 0;
}

Arena::~Arena() {
    for(size_t i = 0; i < blocks_arr.size(); i++) {
        delete[] blocks_arr[i];
    }
}

char* Arena::AllocateNewBlock(size_t block_size) {
    char* ret = new char[block_size];
    blocks_arr.push_back(ret);
    blocks_memory += block_size;

    return ret;
}

char* Arena::Reallocate(size_t size_) {
    if(size_ > (kBlockSize / 4)) {
        // > 1KB, Allocate it separately to avoid wasting too much space in leftover bytes.
        char* ret = AllocateNewBlock(size_);
        return ret;
    }

    // alloc_bytes_remaining < 1KB, allocate a new block with kBlockSize(4KB), waste the 
    // remaining space in the current block.
    alloc_ptr = AllocateNewBlock(kBlockSize);
    char* ret = alloc_ptr;
    alloc_bytes_remaining = kBlockSize;
    alloc_ptr += size_;
    alloc_bytes_remaining -= size_;

    return ret;
}

char* Arena::AllocateAligned(size_t size_) {
    const int align = (sizeof(void*) > 8) ? sizeof(void*) : 8;

    // pointer size should be a power of 2 (2的整数次幂的二进制表示中有且只有一位是1)
    assert((align & (align - 1)) == 0);

    // (alloc_ptr) & (align - 1) ==> alloc_ptr % align
    size_t current_mod = reinterpret_cast<uintptr_t>(alloc_ptr) & (align - 1);

    // slop: additional(new) size is required to align memory
    size_t slop = (current_mod == 0 ? 0 : align - current_mod);
    size_t needed = size_ + slop;

    char* ret = nullptr;
    if(needed <= alloc_bytes_remaining) {
        ret = alloc_ptr + slop;
        alloc_ptr += needed;
        alloc_bytes_remaining -= needed;
    }
    else {
        ret = Reallocate(size_);
    }

    assert((reinterpret_cast<uintptr_t>(ret) & (align - 1)) == 0);
    return ret;
}


}