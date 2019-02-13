#include "util/arena.h"
#include "util/testharness.h"

namespace leveldb {

class ArenaTest { };

TEST(ArenaTest, Empty) {
    Arena arena;
}

TEST(ArenaTest, SimpleAllocate) {
    
}

}

int main(int argc, char** argv) {
    return leveldb::test::RunAllTests();
}