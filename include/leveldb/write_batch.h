#ifndef LEVELDB_INCLUDE_WRITE_BATCH_H_
#define LEVELDB_INCLUDE_WRITE_BATCH_H_

#include <string>
#include "leveldb/status.h"
#include "db/db_format.h"
#include "db/memtable.h"

namespace leveldb {

class WriteBatch {
public:
    WriteBatch();

    WriteBatch(const WriteBatch&) = default;
    WriteBatch& operator =(const WriteBatch&) = default;

    ~WriteBatch();

    // insert the mapping "key->value" into the database
    void Put(const Slice& key, const Slice& value);
    
    // if the database contains a mapping for "key", erase it.  Else do nothing
    void Delete(const Slice& key);

    // clear all updates buffered in this batch
    void Clear();

    size_t BatchSize();

    void Append(const WriteBatch& src);

    class Handler {
    public: 
        SequenceNumber sequence;
        MemTable* mem;

        void Put(const Slice& key, const Slice& value);
        void Delete(const Slice& key);
    };    

private: 
    std::string record;
};

}



#endif