#ifndef LEVELDB_DB_MEMTABLE_H_
#define LEVELDB_DB_MEMTABLE_H_

#include <string>
#include "leveldb/db.h"
#include "db/db_format.h"
#include "db/skiplist.h"
#include "util/arena.h"

namespace leveldb {

class InternalKeyCompactor;

class MemTable {
public: 
    explicit MemTable(const InternalKeyCompactor& compactor);
    void Add(SequenceNumber sequence, ValueType type, const Slice& key, const Slice& value);

private: 
    struct KeyCompactor {
        const InternalKeyCompactor compactor;
        explicit KeyCompactor(const InternalKeyCompactor& comp) : compactor(comp) { }
        int operator()(const char* a, const char* b) const;
    };

    typedef SkipList<const char*, KeyCompactor> Table;
    KeyCompactor comp;
    Arena arena;
    Table table;

    MemTable(const MemTable&);
    void operator=(const MemTable&);
};

}


#endif