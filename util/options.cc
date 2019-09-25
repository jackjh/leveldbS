#include "leveldb/options.h"
#include "leveldb/comparator.h"

namespace leveldb {

Options::Options() : _comparator(ByteComparator()),
                     create_if_missing(false),
                     error_if_exists(false), 
                     error_checks(false),
                     wirte_buffer_size(4 << 20),
                     max_open_files(1000),
                     block_size(4096),
                     max_file_size(2 << 20) {

}

}