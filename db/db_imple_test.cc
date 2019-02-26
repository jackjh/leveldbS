#include "db/db_imple.h"
#include "util/testharness.h"

namespace leveldb {

class DbImpleTest { };

TEST(DbImpleTest, Empty) {
    Options options;
    DBImple* db_imple = new DBImple(options, "dbName");
}

}

int main(int argc, char** argv) {
    return leveldb::test::RunAllTests();
}