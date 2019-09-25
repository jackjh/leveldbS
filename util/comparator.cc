#include "leveldb/comparator.h"
#include "leveldb/slice.h"

namespace leveldb{

Comparator::~Comparator() { }

namespace {

class ByteComparatorImple : public Comparator {
public: 
    ByteComparatorImple() { }

    virtual int Compare(const Slice& s1, const Slice& s2) const {
        return s1.Compare(s2);
    }

    virtual const char* ComparName() const {
        return "leveldb::ByteComparatorImple";
    }

    virtual void FindShortSeparator(std::string* start, const Slice& limit) const {

    }

    virtual void FindShortSelf(std::string* key) const {

    }

};

}   // namespace

static const Comparator* byteCompar;
const Comparator* ByteComparator() {
    byteCompar = new ByteComparatorImple();
    
    return byteCompar;
}

}   // namespace leveldb