#include "util/testharness.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <vector>
#include <string>

namespace leveldb {

namespace test {

namespace {
struct Test {
    const char* base;
    const char* name;
    void (*func)();
};
std::vector<Test>* tests_;
}

bool RegisterTest(const char* base, const char* name, void (*func)()) {
    if(tests_ == nullptr) {
        tests_ = new std::vector<Test>;
    }

    Test t_;
    t_.base = base;
    t_.name = name;
    t_.func = func;
    tests_->push_back(t_);

    return true;
}

int RunAllTests() {
    const char* matcher = getenv("LEVELDB_TESTS");

    int nums = 0;
    if(tests_ != nullptr) {
        for(size_t i = 0; i < tests_->size(); i++) {
            const Test& t = (*tests_)[i];
            if(matcher != nullptr) {
                std::string test_name = t.base;
                test_name.push_back('.');
                test_name.append(t.name);
                if(strstr(test_name.c_str(), matcher) == nullptr) {
                    continue;
                }
            }
            fprintf(stderr, "==== Test %s.%s\n", t.base, t.name);
            (*t.func)();
            ++nums;
        }
    }
    fprintf(stderr, "==== PASSED %d tests\n", nums);
    
    return 0;
}

int RandomSeed() {
    const char* env = getenv("TEST_RANDOM_SEED");
    int ret = (env != nullptr) ? atoi(env) : 301;
    if(ret <= 0) {
        ret = 301;
    }
    return ret;
}


}

}