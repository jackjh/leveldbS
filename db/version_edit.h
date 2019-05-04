#ifndef LEVELDB_DB_VERSION_EDIT_H_
#define LEVELDB_DB_VERSION_EDIT_H_

#include <stdint.h>
#include "db/db_format.h"

namespace leveldb {

struct FileMetaData {
    int refs;
    int allowedSeeks;
    uint64_t fileNumber;    // SSTable文件（.sst）编号
    uint64_t fileSize;      // 文件大小（bytes）
    InternalKey smalllest;  // 该SSTable文件内的最小key
    InternalKey largest;    // 该SSTable文件内的最大key
};

class VersionEdit {
public: 
    VersionEdit() { }
    ~VersionEdit() { }

private:

};

}


#endif
