#ifndef LEVELDB_INCLUDE_SLICE_H_
#define LEVELDB_INCLUDE_SLICE_H_

#include <string>
#include <assert.h>
#include <string.h>

namespace leveldb {

class Slice {
public:
    
    Slice() : _data(""), _size(0) { }
    
    Slice(const char* _d, int n) : _data(_d), _size(n) { }

    Slice(const char* _arr) : _data(_arr), _size(strlen(_arr)) { }

    Slice(const std::string& str) : _data(str.data()), _size(str.size()) { }

    const char* data() const {
        return _data;
    }

    size_t size() const {
        return _size;
    }

    bool empty() const {
        return _size == 0;
    }

    char operator[](size_t n) const {
        assert(n < size());
        return _data[n];
    }

    void clear() {
        _data = "";
        _size = 0;
    }

    void RemovePrefix(size_t n) {
        assert(n < _size);
        _data += n;
        _size -= n;
    }

    std::string ToString() const {
        return std::string(_data, _size);
    }

    // 如果 s 为 *this 的前缀，则返回true
    bool isPrefix(const Slice& s) const {
        return ((s.size() <= _size) && (memcmp(s.data(), _data, s.size()) == 0));
    }

    /*
    ** if *this < s     return -1
    ** if *this == s    return 0
    ** if *this > s     return 1;
    */
    int Compare(const Slice& s) const;

    /*
    ** if a < b     return -1
    ** if a == b    return 0
    ** if a > b     return 1;
    */
    int Compare(const Slice& a, const Slice& b) const;

private:
    const char* _data;
    size_t _size;
};

inline bool operator==(const Slice& a, const Slice& b) {
    return ((a.size() == b.size()) && (memcmp(a.data(), b.data(), a.size()) == 0));
}

inline bool operator!=(const Slice& a, const Slice& b) {
    return !(a == b);
}

inline int Slice::Compare(const Slice& s) const {
    const size_t minSize = (s.size() < _size) ? s.size() : _size;
    int res = memcmp(s.data(), _data, minSize);
    
    if(res == 0) {
        if(_size < s.size()) {
            res = -1;
        }
        else if(_size > s.size()) {
            res = 1;
        }
    }

    return res;
}

inline int Slice::Compare(const Slice& a, const Slice& b) const {
    const size_t minSize = (a.size() < b.size()) ? a.size() : b.size();
    int res = memcmp(a.data(), b.data(), minSize);

    if(res == 0){
        if(a.size() < b.size()) {
            res = -1;
        }
        if(a.size() > b.size()) {
            res = 1;
        }
    }

    return res;
}

}  // end of namespace


#endif