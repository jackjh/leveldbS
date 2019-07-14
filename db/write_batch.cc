#include "leveldb/write_batch.h"
#include "util/coding.h"

namespace leveldb {

static const size_t kHeader = 12;

WriteBatchS::WriteBatchS() {
    Clear();
}

void WriteBatchS::Clear() {
    record.clear();
    record.resize(kHeader);
}

int WriteBatchS::Count(const WriteBatchS* batch) {
    return DecodeFixed32(batch->record.data() + 8);
}

void WriteBatchS::SetCount(WriteBatchS* batch, int n) {
    EncodeFixed32(&batch->record[8], n);
}

SequenceNumber WriteBatchS::Sequence(const WriteBatchS* batch) {
    return SequenceNumber(DecodeFixed64(batch->record.data()));
}

void WriteBatchS::SetSequenceNumber(WriteBatchS* batch, SequenceNumber seq) {
    EncodeFixed64(&batch->record[0], seq);
}

void WriteBatchS::Handler::Put(const Slice& key, const Slice& value) {
    mem->Add(sequence, kTypeValue, key, value);
    sequence++;
}

void WriteBatchS::Handler::Delete(const Slice& key) {
    mem->Add(sequence, kTypeDeletion, key, Slice());
    sequence++;
}

}