#ifndef LEVELDB_DB_MEMTABLE_H_
#define LEVELDB_DB_MEMTABLE_H_

#include <string>
#include "leveldb/db.h"
#include "db/db_format.h"
#include "db/skiplist.h"
#include "util/arena.h"

namespace leveldb {

class MemTable {
public: 
    void Add(SequenceNumber sequence, ValueType type, const Slice& key, const Slice& value);

private: 
    struct KeyComparator {

    };

    typedef SkipList<const char*, KeyComparator> Table;
    Table table;

    MemTable(const MemTable&);
    void operator=(const MemTable&);
};

}


#endif