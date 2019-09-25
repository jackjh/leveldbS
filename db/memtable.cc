#include "db/memtable.h"
#include "util/coding.h"

namespace leveldb {

MemTable::MemTable(const InternalKeyCompactor& compactor)
    : comp(compactor), table(comp, &arena) { }

static Slice GetSliceByLenPrefixed(const char* data) {
    uint32_t len = 0;
    const char* ptr = data;
    ptr = GetVarint32Ptr(ptr, ptr + 5, &len);
    return Slice(ptr, len);
}

int MemTable::KeyCompactor::operator()(const char* a, const char* b) const {
    Slice aKey = GetSliceByLenPrefixed(a);
    Slice bKey = GetSliceByLenPrefixed(b);
    
    return compactor.Compare(aKey, bKey);
}

/*
** the format of an entry into skipList:
** internal_key_size(varint32) | key.data | sequenceNumber|valueType | value size(varint32) | value.data
** internal_key_size = key.size + 8;       sequenceNumber(7 bytes)|valueType(1 byte) => 8 bytes
*/
void MemTable::Add(SequenceNumber sequence, ValueType type, const Slice& key, const Slice& value) {
    size_t keySize = key.size();
    size_t valSize = value.size();

    size_t internalKeySize = keySize + 8;
    size_t entrySize = VarintLen(internalKeySize) + internalKeySize + VarintLen(valSize) + valSize;
    char* buf = arena.AllocateAligned(entrySize);
    char* ptr = EncodeVarint32(buf, internalKeySize);
    memcpy(ptr, key.data(), keySize);
    ptr += keySize;
    EncodeFixed32(ptr, (sequence << 8)|type);
    ptr += 8;
    ptr = EncodeVarint32(ptr, valSize);
    memcpy(ptr, value.data(), valSize);
    assert((ptr + valSize) == (buf + entrySize));
    table.Insert(buf);   
}

}