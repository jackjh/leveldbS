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
    std::vector<std::pair<size_t, char*> > allocated;
    const int N = 100;
    Random rand(301);
    size_t bytes = 0;
    for(int i=0; i<N; i++) {
        size_t s;
        if(i == 0) {
            s = 1;
        }
        else {
            s = i;
        }
        char* buf;
        if(rand.OneIn(10)) {
            buf = arena.AllocateAligned(s);
        }
        else {
            buf = arena.Allocate(s);
        }

        for(size_t j = 0; j < s; j++) {
            buf[j] = i % 256;
        }
        bytes += s;
        allocated.push_back(std::make_pair(s, buf));
        ASSERT_GEQ(arena.MemoryUsage(), bytes);
    }
}

}

int main(int argc, char** argv) {
    return leveldb::test::RunAllTests();
}