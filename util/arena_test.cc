#include "util/arena.h"
#include "util/testharness.h"
#include "util/random.h"

namespace leveldb {

class ArenaTest { };

TEST(ArenaTest, Empty) {
    Arena arena;
}

TEST(ArenaTest, SimpleAllocate) {
    Arena arena;
    const int N = 100000;
    Random ran(301);
}

}

int main(int argc, char** argv) {
    return leveldb::test::RunAllTests();
}