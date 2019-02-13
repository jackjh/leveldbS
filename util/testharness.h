#ifndef LEVELDB_UTIL_TESTHARNESS_H_
#define LEVELDB_UTIL_TESTHARNESS_H_

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include "leveldb/status.h"

namespace leveldb {

namespace test {

// return 0 if all tests pass
int RunAllTests();

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
                ss << " " << s.toString();
                tOk = false;
            }
            return *this;
        }

#define BINARY_OP(name, op)                                  \
        template <class X, class Y>                          \
        Tester& name(const X& x, const Y& y) {               \
            if(!(x op y)) {                                  \
                ss << "failed: " << x << (" " #op " ") << y; \
                tOk = false;                                 \
            }                                                \
            return *this;                                    \
        }
        
        BINARY_OP(IsEq,  ==)
        BINARY_OP(IsNEq, !=)
        BINARY_OP(IsGEq, >=)
        BINARY_OP(IsGt,  >)
        BINARY_OP(IsLEq, <=)
        BINARY_OP(IsLt,  <)
#undef BINARY_OP

        template<class T>
        Tester& operator<<(const T& value) {
            if(!tOk) {
                ss << " " << value;
            }
            return *this;
        }
};

#define ASSERT_TRUE(c) ::leveldb::test::Tester(__FILE__, __LINE__).Is((c), #c)
#define ASSERT_OK(s) ::leveldb::test::Tester(__FILE__, __LINE__).IsOk((s))
#define ASSERT_EQ(a, b) ::leveldb::test::Tester(__FILE__, __LINE__).IsEq((a), (b))
#define ASSERT_NEQ(a, b) ::leveldb::test::Tester(__FILE__, __LINE__).IsNEq((a), (b))
#define ASSERT_GEQ(a, b) ::leveldb::test::Tester(__FILE__, __LINE__).IsGEq((a), (b))
#define ASSERT_GT(a, b) ::leveldb::test::Tester(__FILE__, __LINE__).IsGt((a), (b))
#define ASSERT_LEQ(a, b) ::leveldb::test::Tester(__FILE__, __LINE__).IsLEq((a), (b))
#define ASSERT_LT(a, b) ::leveldb::test::Tester(__FILE__, __LINE__).IsLt((a), (b))

#define TCONCAT(a, b) TCONCAT_1(a, b)
#define TCONCAT_1(a, b)  a##b


#define TEST(base, name)                            \
class TCONCAT(_Test_, name) : public base {          \
public:                                             \
    void _Run();                                     \
    static void _RunIt() {                           \
        TCONCAT(_Test_, name) t;                     \
        t._Run();                                    \
    }                                               \
};                                                  \
bool TCONCAT(_Test_ignored_, name) =                \
        ::leveldb::test::RegisterTest(#base, #name, &TCONCAT(_Test_, name)::_RunIt);   \
void TCONCAT(_Test_, name)::_Run()


bool RegisterTest(const char* base, const char* name, void (*func)());

}   // namespace test

}   // namespace leveldb


#endif