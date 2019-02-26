#ifndef LEVELDB_DB_DB_IMPLE_H_
#define LEVELDB_DB_DB_IMPLE_H_

#include <deque>
#include "leveldb/db.h"

namespace leveldb {

class DBImple : public DB {
public: 
    DBImple(const Options& options, const std::string& dbName) { }
    virtual ~DBImple() { }

    //virtual Status 

private:
    DBImple(const DBImple&);
    void operator=(const DBImple&);
};

}



#endif