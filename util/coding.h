#ifndef LEVELDB_SIMP_CODING_H_
#define LEVELDB_SIMP_CODING_H_

#include <stdint.h>
#include <string>
#include <string.h>
#include "leveldb/slice.h"

namespace leveldb {

// convert value to little endian
extern void EncodeFixed32(char* buf, uint32_t value);
extern void EncodeFixed64(char* buf, uint64_t value);

// append the value(EncodeFixed) into the dst
extern void PutFixed32(std::string* dst, uint32_t value);
extern void PutFixed64(std::string* dst, uint64_t value);

// Varint, save space
extern char* EncodeVarint32(char* buf, uint32_t value);
extern char* EncodeVarint64(char* buf, uint64_t value);

// append the value(EncodeVarint) into the dst
extern void PutVarint32(std::string* dst, uint32_t value);
extern void PutVarint64(std::string* dst, uint64_t value);
extern void PutLenDataSlice(std::string* dst, const Slice& s);

extern bool GetVarint32(Slice* input, uint32_t* value);
extern bool GetVarint64(Slice* input, uint64_t* value);

extern const char* GetVarint32PtrFallBack(const char* p, const char* l, uint32_t* value);
extern const char* GetVarint32Ptr(const char* p, const char* l, uint32_t* value);
extern const char* GetVarint64Ptr(const char* p, const char* l, uint64_t* value);

// return the bytes of the uint32_t or uint64_t (after encoding of "value")
extern int VarintLen(uint64_t value);

extern bool GetLenPreFixedSlice(Slice* input, Slice* res);
extern const char* GetLenPreFixedSlice(const char* p, const char* l, Slice* res);

inline uint32_t DecodeFixed32(const char* buf) {
    #if __BYTE_ORDER == __LITTLE_ENDIAN
        uint32_t res;
        memcpy(&res, buf, sizeof(res));
        return res;
    #else
        return ((static_cast<uint32_t>(static_cast<unsigned char>(buf[0]))) | 
                (static_cast<uint32_t>(static_cast<unsigned char>(buf[1])) << 8) |
                (static_cast<uint32_t>(static_cast<unsigned char>(buf[2])) << 16) |
                (static_cast<uint32_t>(static_cast<unsigned char>(buf[3])) << 24));
    #endif
}
inline uint64_t DecodeFixed64(const char* buf) {
     #if __BYTE_ORDER == __LITTLE_ENDIAN
        uint64_t res;
        memcpy(&res, buf, sizeof(res));
        return res;
    #else
        uint64_t low = DecodeFixed32(buf);
        uint64_t high = DecodeFixed64(buf + 4);
        return (high << 32) | low;
    #endif
}

}


#endif