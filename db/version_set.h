#ifndef LEVELDB_DB_VERSION_SET_H_
#define LEVELDB_DB_VERSION_SET_H_

#include <vector>
#include <set>
#include "db/db_format.h"
#include "db/version_edit.h"

namespace leveldb {

class Version {
private: 
    Version* prev;
    Version* next;

    std::vector<FileMetaData*> files[config::kNumLevels];
};

}


#endif