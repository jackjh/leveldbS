#include "leveldb/write_batch.h"
#include "util/testharness.h"

namespace leveldb {

class WriteBatchTest { };

TEST(WriteBatchTest, empty_constructor) {
    WriteBatch batch;
}

}   // namespace leveldb

int main(int argc, char** argv) {
    return leveldb::test::RunAllTests();
}