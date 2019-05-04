#ifndef LEVELDB_DB_DB_FORMAT_H_
#define LEVELDB_DB_DB_FORMAT_H_

#include <string>
#include "leveldb/slice.h"

namespace leveldb {

namespace config {

// leveldb中的层数
static const int kNumLevels = 7;

// 当level 0中达到该文件个数时，会触发level 0的compaction
static const int kL0CompactionTrigger = 4;

// 当level 0中达到该文件个数时，会减慢写入的速度
static const int KL0SlowdownWritesTrigger = 8;

// level 0最大的文件个数，当被触发时，停止写入
static const int KL0StopWritesTrigger = 12;

// 对memtable进行合并时，最多只能合并到level 2
static const int KMaxMemtableCompactLevel = 2;

}

enum ValueType {
    kTypeDeletion = 0x0,
    kTypeValue = 0x1
};

typedef uint64_t SequenceNumber;

struct ParsedInternalKey {
    Slice userKey;
    SequenceNumber seqNum;
    ValueType valType;

    ParsedInternalKey() { }

    ParsedInternalKey(const Slice& u_key, SequenceNumber s_num, ValueType type)
            : userKey(u_key), seqNum(s_num), valType(type) {
        
    }
};

// insert the key into the record
void AppendInternalKey(std::string* record, const ParsedInternalKey& key);

inline Slice ExtractUserKey(const Slice& internal_key) {
    assert(internal_key.size() >= 8);
    return Slice(internal_key.data(), internal_key.size() - 8);
}

/*
** InternalKey::record  => 
** | user key (string) | sequence number (7-bytes) | value type (1-byte) |
*/
class InternalKey {
private: 
    std::string record;

public:
    InternalKey() { }
    InternalKey(const Slice& userKey, SequenceNumber seq, ValueType valType) {
        AppendInternalKey(&record, ParsedInternalKey(userKey, seq, valType));
    }

    void SetKeyFromStart(const ParsedInternalKey& key) {
        record.clear();
        AppendInternalKey(&record, key);
    }

    Slice UserKey() {
        return ExtractUserKey(record);
    }

    Slice Encode() {
        assert(!record.empty());
        return record;
    }

    void Decode(const Slice& s) {
        record.assign(s.data(), s.size());
    }

    void Clear() { record.clear(); }
};

}



#endif