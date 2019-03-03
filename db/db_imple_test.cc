#include "db/db_imple.h"
#include "util/testharness.h"

namespace leveldb {

class DBImpleTest { };

TEST(DBImpleTest, Empty) {
    DBImple db_imple;
}

TEST(DBImpleTest, with_Options) {
    Options options;
    DBImple* db_imple_ptr = new DBImple(options, "dbname");
}


} // namespace leveldb

int main(int argc, char** argv) {
    return leveldb::test::RunAllTests();
}