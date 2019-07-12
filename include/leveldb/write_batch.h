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

    Status Iterate(Handler* handler) const;

private:
    // return the number of the entries in this batch
    static int Count(WriteBatch* batch);

    // set the count for the number of the entries in this batch
    static void SetCount(WriteBatch* batch, int n);

private: 
/*
**  Put(Slice& key, Slice& value);
** the format of the WriteBatch::record is :
** sequence (8-bytes) | count(the num of the entries in the batch) (4-bytes) | data[count]
**
** the data[count] :  kTypeValue | key.size() | key.data() | value.size() | value.data()  
**            or :    kTypeDeletion | key.size() | key.data()
*/

    std::string record;
};

}



#endif