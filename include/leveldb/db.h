#ifndef LEVELDB_INCLUDE_DB_H_
#define LEVELDB_INCLUDE_DB_H_

#include <string>

namespace leveldb {

class DB {
public:
    DB() { }
    virtual ~DB();

private: 
    DB(const DB&);
    void operator=(const DB&);
};

}



#endif