#include "leveldb/write_batch.h"

namespace leveldb {

const static size_t kHeader = 12;

WriteBatch::WriteBatch() {
    Clear();
}

WriteBatch::~WriteBatch() { }

void WriteBatch::Clear() {
    record.clear();
    record.resize(kHeader);
}

}