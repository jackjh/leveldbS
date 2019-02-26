#ifndef LEVELDB_INCLUDE_DB_H_
#define LEVELDB_INCLUDE_DB_H_

#include <string>
#include "leveldb/options.h"
#include "leveldb/status.h"

namespace leveldb {

class DB {
public:
    DB() { }
    virtual ~DB() { }

    static Status Open(const Options& options, const std::string& dbName, DB** dbPtr);

    virtual Status Put(const WriteOptions& wOptions, const Slice& key, const Slice& value) = 0;

    virtual Status Get(const ReadOptions& rOptions, const Slice& key, std::string* value) = 0;

private: 
    DB(const DB&);
    void operator=(const DB&);
};

}



#endif