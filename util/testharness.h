#ifndef LEVELDB_UTIL_TESTHARNESS_H_
#define LEVELDB_UTIL_TESTHARNESS_H_

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include "leveldb/status.h"

namespace leveldb {

namespace test {

// return 0 if all tests pass
int RunAllTest();

std::string TempDir();

int RandomSeed();

class Tester{
    private: 
        bool tOk;
        const char* fname;
        int line;
        std::stringstream ss;
    
    public: 
        Tester(const char* f, int l) : tOk(true), fname(f), line(l) { }
        ~Tester() {
            if(!tOk) {
                fprintf(stderr, "%s:%d:%s\n", fname, line, ss.str().c_str());
                exit(1);
            }
        }

        Tester& Is(bool b, const char* msg) {
            if(!b) {
                ss << "Assertion failure" << msg;
                tOk = false;
            }
            return *this;
        }

        Tester& IsOk(const Status& s) {
            if(!s.ok()) {
                ss << " " << s.ToString();
                tOk = false;
            }
            return *this;
        }

        template<class T>
        Tester& operator<<(const T& value) {
            if(!tOk) {
                ss << " " << value;
            }
            return *this;
        }
};

}

}


#endif