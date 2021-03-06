#ifndef LEVELDB_DB_DB_IMPLE_H_
#define LEVELDB_DB_DB_IMPLE_H_

#include <deque>
#include "leveldb/db.h"
#include "port/port_stdcxx.h"
#include "db/memtable.h"
#include "db/db_format.h"

namespace leveldb {

class MemTable;

class DBImple : public DB {
public: 
    DBImple() { }
    DBImple(const Options& options, const std::string& dbName);
    virtual ~DBImple();

    virtual Status Put(const WriteOptions& wOptions, const Slice& key, const Slice& value);

    virtual Status Get(const ReadOptions& rOptions, const Slice& key, std::string* value); 

    virtual Status Write(const WriteOptions& options, WriteBatchS* batch);

private:
    friend class DB;
    port::Mutex mu;

    struct Writer;

    const InternalKeyCompactor internalComp;
    
    MemTable* memtab;

    DBImple(const DBImple&);
    void operator=(const DBImple&);
};

}



#endif