#include "util/arena.h"

namespace leveldb {

static const int kBlockSize = 4096;

Arena::Arena() {
    block_memory = 0;
    alloc_ptr = NULL;
    alloc_bytes_remaining  = 0;
}

Arena::~Arena() {
    for(size_t i = 0; i < blocks_arr.size(); i++) {
        delete[] blocks_arr[i];
    }
}

}