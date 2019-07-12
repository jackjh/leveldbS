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

void WriteBatch::Handler::Put(const Slice& key, const Slice& value) {
    mem->Add(sequence, kTypeValue, key, value);
    sequence++;
}

void WriteBatch::Handler::Delete(const Slice& key) {
    mem->Add(sequence, kTypeDeletion, key, Slice());
    sequence++;
}

}