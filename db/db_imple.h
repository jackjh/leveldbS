#ifndef LEVELDB_DB_DB_IMPLE_H_
#define LEVELDB_DB_DB_IMPLE_H_

#include <deque>
#include "leveldb/db.h"

namespace leveldb {

class DBImple : public DB {
    DBImple(const std::string& dbName);
    virtual ~DBImple();
};

}



#endif