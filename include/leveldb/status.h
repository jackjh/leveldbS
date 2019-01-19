#ifndef LEVELDB_INCLUDE_STATUS_H_
#define LEVELDB_INCLUDE_STATUS_H_

#include <algorithm>
#include <string>
#include "leveldb/slice.h"

namespace leveldb {

class Status {
public:
    Status() : status_(nullptr) { }

    ~Status() {
        delete[] status_;
    }

    Status(const Status& st);
    Status& operator=(const Status& st);

    Status(Status&& st) : status_(st.status_) {
        st.status_ = nullptr;
    }
    Status& operator=(Status&& st);

    // return a success status
    static Status OK() { 
        return Status(); 
    }

    // return error status
    static Status NotFound(const Slice& s1, const Slice& s2 = Slice()) {
        return Status(kNotFound, s1, s2);
    }

    static Status Corruption(const Slice& s1, const Slice& s2 = Slice()) {
        return Status(kCorruption, s1, s2);
    }

    static Status NotSupported(const Slice& s1, const Slice& s2 = Slice()) {
        return Status(kNotSupported, s1, s2);
    }
    
    static Status InvalidArgument(const Slice& s1, const Slice& s2 = Slice()) {
        return Status(kInvalidArgument, s1, s2);
    }
    static Status IOError(const Slice& s1, const Slice& s2 = Slice()) {
        return Status(kIOError, s1, s2);
    }

    bool ok() const {
        return (status_ == nullptr);
    }

    bool isNotFound() const {
        return (code() == kNotFound);
    }

    bool isCorruption() const {
        return (code() == kCorruption);
    }
    bool isNotSupported() const {
        return (code() == kNotSupported);
    }

    bool isInvalidArgument() const {
        return (code() == kInvalidArgument);
    }

    bool isIOError() const {
        return (code() == kIOError);
    }

    std::string toString() const;

private: 
/*
** OK status has a null status_.
** state_[0..3] == length of message
** state_[4]    == code
** state_[5..]  == message
*/
    char* status_;
    
    enum Code {
        kOk = 0,
        kNotFound,
        kCorruption,
        kNotSupported,
        kInvalidArgument,
        kIOError
    };

    Status(Code c, const Slice& s1, const Slice& s2);
    static char* CopyStatus(const char* st);

    Code code() const {
        return (status_ == nullptr) ? kOk : static_cast<Code>(status_[4]);
    }
};

inline Status::Status(const Status& st) {
    status_ = (st.status_ == nullptr) ? nullptr : CopyStatus(st.status_);
}

inline Status& Status::operator=(const Status& st) {
    if (status_ != st.status_) {
        delete[] status_;
        status_ = (st.status_ == nullptr) ? nullptr : CopyStatus(st.status_);
    }
    return *this;
}

inline Status& Status::operator=(Status&& st) {
    std::swap(status_, st.status_);
    return *this;
}

}


#endif