#ifndef LEVELDB_INCLUDE_WRITE_BATCH_S_H_
#define LEVELDB_INCLUDE_WRITE_BATCH_S_H_

#include <string>
#include "leveldb/status.h"
#include "db/db_format.h"
#include "db/memtable.h"

namespace leveldb {

class WriteBatchS {
public:
    WriteBatchS();

    WriteBatchS(const WriteBatchS&) = default;
    WriteBatchS& operator =(const WriteBatchS&) = default;

    ~WriteBatchS() { }

    // insert the mapping "key->value" into the database
    void Put(const Slice& key, const Slice& value);
    
    // if the database contains a mapping for "key", erase it.  Else do nothing
    void Delete(const Slice& key);

    // clear all updates buffered in this batch
    void Clear();

    size_t BatchSSize();

    void Append(const WriteBatchS& src);

    class Handler {
    public: 
        SequenceNumber sequence;
        MemTable* mem;

        void Put(const Slice& key, const Slice& value);
        void Delete(const Slice& key);
    };

    Status Iterate(Handler* handler) const;

    Status InsertInto(MemTable* mem);

    // return the number of the entries in this batch
    int Count() const;

    // set the count for the number of the entries in this batch
    void SetCount(int n);

    SequenceNumber Sequence() const;

    void SetSequenceNumber(SequenceNumber seq);
    
    Slice BatchContents();

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