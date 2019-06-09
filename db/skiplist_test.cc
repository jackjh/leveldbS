#include "db/skiplist.h"
#include "util/testharness.h"

namespace leveldb {

typedef uint64_t Key;

struct Comparator {
    int operator()(const Key& a, const Key& b) const {
        if(a < b) {
            return -1;
        }
        else if(a > b) {
            return 1;
        }
        else {
            return 0;
        }
    }
};

class SkipListTest { };

TEST(SkipListTest, empty_constructor) {
    Arena arena;
    Comparator cmp;
    SkipList<Key, Comparator> sList(cmp, &arena);
    ASSERT_TRUE(!sList.Contains(10));

    SkipList<Key, Comparator>::Iterator iter(&sList);
    ASSERT_TRUE(!iter.Valid());
}

}

int main(int argc, char** argv) {
    return leveldb::test::RunAllTests();
}