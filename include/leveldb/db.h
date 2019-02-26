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

    //virtual Status Put()

private: 
    DB(const DB&);
    void operator=(const DB&);
};

}



#endif