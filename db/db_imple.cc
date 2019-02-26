#include "db/db_imple.h"

namespace leveldb {

Status DBImple::Put(const WriteOptions& wOptions, const Slice& key, const Slice& value) {
    return DB::Put(wOptions, key, value);
}

Status DBImple::Get(const ReadOptions& rOptions, const Slice& key, std::string* value) {
    Status s;

    return s;
}

Status DB::Put(const WriteOptions& wOptions, const Slice& key, const Slice& value) {
    Status s;

    return s;
}

Status DB::Open(const Options& options, const std::string& dbName, DB** dbPtr) {
    Status s;
    
    return s;
}

}