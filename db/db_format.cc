#include "db/db_format.h"
#include "util/coding.h"

namespace leveldb {

uint64_t PackSequenceAndType(SequenceNumber seq, ValueType type) {
    assert(seq <= maxSequenceNumber);
    assert(type <= valueTypeSeek);

    return ((seq << 8) | type);
}

void AppendInternalKey(std::string* record, const ParsedInternalKey& key) {
    record->append(key.userKey.data(), key.userKey.size());
    uint64_t val = PackSequenceAndType(key.seqNum, key.valType);
    PutFixed64(record, val);
}

// Order by:
//    increasing user key (according to user-supplied comparator)
//    decreasing sequence number
//    decreasing type (though sequence# should be enough to disambiguate)
int InternalKeyCompactor::Compare(const Slice& a, const Slice& b) const {
    int res = a.Compare(b);
    if(0 == res) {
        uint64_t aNum = DecodeFixed64(a.data() + a.size() - 8);
        uint64_t bNum = DecodeFixed64(b.data() + b.size() - 8);

        if(aNum > bNum) {
            return -1;
        }
        else if(aNum < bNum) {
            return 1;
        }
    }

    return res;
}

int InternalKeyCompactor::Compare(const InternalKey& a, const InternalKey& b) const {
    Slice aKey = a.Encode();
    Slice bKey = b.Encode();

    return Compare(aKey, bKey);
}

}