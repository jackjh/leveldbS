#include "db/version_edit.h"
#include "util/testharness.h"

namespace leveldb {

class VersionEditTest { };

TEST(VersionEditTest, empty_constructot) {
    FileMetaData fileMeta;
}

}   // namespace leveldb

int main(int argc, char** argv) {
    return leveldb::test::RunAllTests();
}