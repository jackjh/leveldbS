#include "db/db_imple.h"
#include "port/port_stdcxx.h"
#include "db/write_batch.cc"
#include "util/mutexlock.h"

namespace leveldb {

struct DBImple::Writer {
    WriteBatchS* batch;
    Status status;
    bool sync;
    bool done;
    port::CondVar cv;

    explicit Writer(port::Mutex* mu) : cv(mu) { }
};

DBImple::DBImple(const Options& options, const std::string& dbName) {

}

DBImple::~DBImple() {

}

Status DBImple::Put(const WriteOptions& wOptions, const Slice& key, const Slice& value) {
    return DB::Put(wOptions, key, value);
}

Status DBImple::Get(const ReadOptions& rOptions, const Slice& key, std::string* value) {
    Status s;

    return s;
}

Status DB::Put(const WriteOptions& wOptions, const Slice& key, const Slice& value) {
    WriteBatchS batch;
    batch.Put(key, value);

    return Write(wOptions, &batch);
}

Status DB::Open(const Options& options, const std::string& dbName, DB** dbPtr) {
    Status s;
    
    return s;
}

Status DBImple::Write(const WriteOptions& options, WriteBatchS* batch) {
    Status s;
    uint64_t lastSequence;

    // get the current sequenceNumber
    lastSequence = options.g_sequence_number;
    if(batch != nullptr) {
        batch->SetSequenceNumber(lastSequence + 1);
    }

    return s;
}

}