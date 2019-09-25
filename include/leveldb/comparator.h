#ifndef LEVELDB_INCLUDE_COMPARATOR_H_
#define LEVELDB_INCLUDE_COMPARATOR_H_

#include <string>

namespace leveldb {

class Slice;

class Comparator {
public:
    virtual ~Comparator();

    /*
    ** s1 > s2, > 0
    ** s1 == s2, = 0
    ** s1 < s2, < 0
    */
    virtual int Compare(const Slice& s1, const Slice& s2) const = 0;
    
    virtual const char* ComparName() const = 0;

    virtual void FindShortSeparator(std::string* start, const Slice& limit) const = 0;
    virtual void FindShortSelf(std::string* key) const = 0;
};

const Comparator* ByteComparator();

}


#endif