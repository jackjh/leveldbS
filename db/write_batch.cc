#include "leveldb/write_batch.h"
#include "util/coding.h"

namespace leveldb {

// kHeader: sequence (8-bytes) | count(the num of the entries in the batch) (4-bytes)
static const size_t kHeader = 12;

WriteBatchS::WriteBatchS() {
    Clear();
}

void WriteBatchS::Clear() {
    record.clear();
    record.resize(kHeader);
}

size_t WriteBatchS::BatchSSize() {
    return record.size();
}

int WriteBatchS::Count() const {
    return DecodeFixed32(record.data() + 8);
}

void WriteBatchS::SetCount(int n) {
    EncodeFixed32(&record[8], n);
}

Status WriteBatchS::Iterate(Handler* handler) const {
    Slice input(record);
    if(input.size() <= kHeader) {
        return Status::Corruption("writeBatch too small");
    }

    input.RemovePrefix(kHeader);
    int batchCnt = 0;
    Slice key, value;
    while(!input.empty()) {
        batchCnt++;
        char valType = input[0];
        input.RemovePrefix(1);
        switch(valType) {
            case kTypeValue:
                if(GetLenPreFixedSlice(&input, &key) && GetLenPreFixedSlice(&input, &value)) {
                    handler->Put(key, value);
                }
                else {
                    return Status::Corruption("write batch put fail.");
                }
                break;
            case kTypeDeletion:
                if(GetLenPreFixedSlice(&input, &key)) {
                    handler->Delete(key);
                }
                else {
                    return Status::Corruption("write batch delete fail.");
                }
                break;
            default:
                return Status::Corruption("unknown write batch type value."); 
        }
    }

    int nums = Count();

    if(batchCnt != nums) {
        return Status::Corruption("write batch get a wrong count.");
    }

    return Status::OK();
}

SequenceNumber WriteBatchS::Sequence() const {
    return SequenceNumber(DecodeFixed64(record.data()));
}

void WriteBatchS::SetSequenceNumber(SequenceNumber seq) {
    EncodeFixed64(&record[0], seq);
}

void WriteBatchS::Handler::Put(const Slice& key, const Slice& value) {
    mem->Add(sequence, kTypeValue, key, value);
    sequence++;
}

void WriteBatchS::Handler::Delete(const Slice& key) {
    mem->Add(sequence, kTypeDeletion, key, Slice());
    sequence++;
}

Status WriteBatchS::InsertInto(MemTable* mem) {
    Handler handler;
    handler.sequence = Sequence();
    handler.mem = mem;
    return Iterate(&handler);
}

void WriteBatchS::Append(const WriteBatchS& src) {
    SetCount(Count() + src.Count());
    assert(src.record.size() >= kHeader);
    record.append(src.record.data() + kHeader, src.record.size() - kHeader);
}

Slice WriteBatchS::BatchContents() {
    return Slice(record);
}

void WriteBatchS::Put(const Slice& key, const Slice& value) {
    SetCount(Count() + 1);
    record.push_back(static_cast<char>(kTypeValue));
    PutLenDataSlice(&record, key);
    PutLenDataSlice(&record, value);
}

void WriteBatchS::Delete(const Slice& key) {
    SetCount(Count() + 1);
    record.push_back(static_cast<char>(kTypeDeletion));
    PutLenDataSlice(&record, key);
}

}