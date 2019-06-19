#include "coding.h"

namespace leveldb {

// convert value to little endian
void EncodeFixed32(char* buf, uint32_t value) {
    #if __BYTE_ORDER == __LITTLE_ENDIAN
        memcpy(buf, &value, sizeof(value));
    #else
        buf[0] = value & 0xff;
        buf[1] = (value >> 8) & 0xff;
        buf[2] = (value >> 16) & 0xff;
        buf[3] = (value >> 24) & 0xff;
    #endif
}

void EncodeFixed64(char* buf, uint64_t value) {
    #if __BYTE_ORDER == __LITTLE_ENDIAN
        memcpy(buf, &value, sizeof(value));
    #else
        buf[0] = value & 0xff;
        buf[1] = (value >> 8) & 0xff;
        buf[2] = (value >> 16) & 0xff;
        buf[3] = (value >> 24) & 0xff;
        buf[4] = (value >> 32) & 0xff;
        buf[5] = (value >> 40) & 0xff;
        buf[6] = (value >> 48) & 0xff;
        buf[7] = (value >> 56) & 0xff;
    #endif
}

void PutFixed32(std::string* dst, uint32_t value) {
    char buf[sizeof(value)];
    EncodeFixed32(buf, value);
    dst->append(buf, sizeof(buf));
}

void PutFixed64(std::string* dst, uint64_t value) {
    char buf[sizeof(value)];
    EncodeFixed64(buf, value);
    dst->append(buf, sizeof(buf));
}

char* EncodeVarint32(char* buf, uint32_t value) {
    unsigned char* p = reinterpret_cast<unsigned char*>(buf);
    static const int B = 128;
    if(value < (1 << 7)) {
        *(p++) = value;
    }
    else if(value < (1 << 14)) {
        *(p++) = value | B;
        *(p++) = value >> 7;
    }
    else if(value < (1 << 21)) {
        *(p++) = value | B;
        *(p++) = (value >> 7) | B;
        *(p++) = value >> 14;
    }
    else if(value < (1 << 28)) {
        *(p++) = value | B;
        *(p++) = (value >> 7) | B;
        *(p++) = (value >> 14) | B;
        *(p++) = value >> 21;
    }
    else {
        *(p++) = value | B;
        *(p++) = (value >> 7) | B;
        *(p++) = (value >> 14) | B;
        *(p++) = (value >> 21) | B;
        *(p++) = value >> 28;
    }

    return reinterpret_cast<char*>(p);
}

char* EncodeVarint64(char* buf, uint64_t value) {
    static const int B = 128;
    unsigned char* p = reinterpret_cast<unsigned char*>(buf);
    while(value >=  B) {
        *(p++) = (value & (B-1)) | B;
        value >>= 7;
    }
    *(p++) = static_cast<unsigned char>(value);
    return reinterpret_cast<char*>(p);
}

void PutVarint32(std::string* dst, uint32_t value) {
    char buf[5];
    char* p = EncodeVarint32(buf, value);
    dst->append(buf, p - buf);
}

void PutVarint64(std::string* dst, uint64_t value) {
    char buf[10];
    char* p = EncodeVarint64(buf, value);
    dst->append(buf, p - buf);
}

void PutLenDataSlice(std::string* dst, const Slice& s) {
    PutVarint32(dst, s.size());
    dst->append(s.data(), s.size());
}

const char* GetVarint32PtrFallBack(const char* p, const char* l, uint32_t* value) {
    uint32_t res = 0;
    for (uint32_t i = 0; i <= 28 && p < l; i += 7) {
        uint32_t tmp = *(reinterpret_cast<const unsigned char*>(p));
        p++;
        if(tmp & 128) {     // > 1-byte (2^7)
            res |= ((tmp & 127) << i);
        }
        else {              // <= 1-byte
            res |= (tmp << i);
            *value = res;
            return reinterpret_cast<const char*>(p);
        }
    }
    return NULL;
}

const char* GetVarint32Ptr(const char* p, const char* l, uint32_t* value) {
    if(p < l) {
        uint32_t result = *(reinterpret_cast<const unsigned char*>(p));
        if((result & 128) == 0) {
            *value = result;
            return p + 1;
        }
    }
    return GetVarint32PtrFallBack(p, l, value);
}

/*
** get the size of the input.data(), the first several bytes
** i.e. WriteBatch::_record    key.size() | key.data()
** get the key.size(), and put into the "value", change the "input" to the key.data()
*/
bool GetVarint32(Slice* input, uint32_t* value) {
    const char* p = input->data();
    const char* l = p + input->size();
    const char* res = GetVarint32Ptr(p, l, value);     // get the key.size()
    if(res == NULL) {               // key.data is NULL
        return false;
    }
    else {
        *input = Slice(res, l - res);           // get the key.data()
        return true;
    }
}

const char* GetVarint64Ptr(const char* p, const char* l, uint64_t* value) {
    uint64_t res = 0;
    for (uint32_t i = 0; i <= 63 && p < l; i += 7) {
        uint64_t tmp = *(reinterpret_cast<const unsigned char*>(p));
        p++;
        if(tmp & 128) {
            res |= ((tmp & 127) << i);
        }
        else {
            res |= (tmp << i);
            *value = res;
            return reinterpret_cast<const char*>(p);
        }
    }
    return NULL;
}

bool GetVarint64(Slice* input, uint64_t* value) {
    const char* p = input->data();
    const char* l = p + input->size();
    const char* res = GetVarint64Ptr(p, l, value);
    if(res == NULL) {
        return false;
    }
    else {
        *input = Slice(res, l - res);
        return true;
    }
}

int VarintLen(uint64_t value) {
    int len = 1;
    while(value >= 128) {
        len += 1;
        value >>= 7;
    }
    return len;
}

bool GetLenPreFixedSlice(Slice* input, Slice* res) {
    uint32_t len;
    bool flag = GetVarint32(input, &len);
    if(flag && input->size() >= len) {
        *res = Slice(input->data(), len);
        input->RemovPrefix(len);
        return true;
    }
    else {
        return false;
    }
}

const char* GetLenPreFixedSlice(const char* p, const char* l, Slice* res) {
    uint32_t len;
    p = GetVarint32Ptr(p, l, &len);
    if(p == NULL) {
        return NULL;
    }
    if(p + len > l) {
        return NULL;
    }

    *res = Slice(p, len);

    return p + len;
}

}