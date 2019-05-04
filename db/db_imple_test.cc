#include "db/db_imple.h"
#include "db/db_format.h"
#include "util/testharness.h"

namespace leveldb {

class DBImpleTest { };

class DBFormatTest { };

TEST(DBImpleTest, Empty) {
    DBImple db_imple;
}

TEST(DBImpleTest, with_Options) {
    Options options;
    DBImple* db_imple_ptr = new DBImple(options, "dbname");
}

TEST(DBFormatTest, empty_construct) {
    InternalKey internal_key;
}

} // namespace leveldb

int main(int argc, char** argv) {
    return leveldb::test::RunAllTests();
}