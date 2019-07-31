#include <stdio.h>
#include "leveldb/status.h"

namespace leveldb {

Status::Status(Code c, const Slice& s1, const Slice& s2) {
    assert(c != kOk);
    int size1 = s1.size();
    int size2 = s2.size();
    int size_ = size1 + (size2) ? (2 + size2) : 0;
    
    char* res = new char[size_ + 5];
    memcpy(res, &size_, sizeof(size_));
    res[4] = static_cast<char>(c);
    memcpy(res + 5, s1.data(), size1);
    if(size2) {
        res[5 + size1] = ':';
        res[6 + size1] = ' ';
        memcpy(res + 7, s2.data(), size2);
    }

    status_ = res;
}

const char* Status::CopyStatus(const char* st) {
    uint32_t size_;
    memcpy(&size_, st, sizeof(size_));
    char* res = new char[size_ + 5];
    memcpy(res, st, size_ + 5);

    return res;
}

std::string Status::toString() const {
    if(status_ == nullptr) {
        return "OK";
    }
    else {
        char tmp[30];
        const char* str;
        switch(code()) {
            case kOk:
                str = "OK";
                break;
            case kNotFound:
                str = "NotFound";
                break;
            case kCorruption:
                str = "Corruption";
                break;
            case kNotSupported:
                str = "NotSupported";
                break;
            case kInvalidArgument:
                str = "InvalidArgument";
                break;
            case kIOError:
                str = "IOError";
                break;
            default:
                snprintf(tmp, sizeof(tmp), "unknown code(%d): ", static_cast<int>(code()));
                str = tmp;
                break;
        }
        std::string res(str);
        uint32_t size_;
        memcpy(&size_, status_, sizeof(size_));
        res.append(status_ + 5, size_);

        return res;
    }
}


}