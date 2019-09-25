#ifndef LEVELDB_INCLUDE_OPTIONS_H_
#define LEVELDB_INCLUDE_OPTIONS_H_

#include <stddef.h>
#include <stdint.h>

namespace leveldb {

class Comparator;

enum compressionType {
    kNoCompression      = 0x0,
    kSnappyCompression  = 0x1
};

struct Options {
    const Comparator* _comparator;

    // if true, the database will be created if it is missing. Default: false
    bool create_if_missing;

    // if true, an error is raised if the database already exists. Default: false
    bool error_if_exists;

    //if true, the implementation will do aggressive checking of the data it is processing
    // and will stop early if it detects any errors. This may have unforeseen ramifications: 
    // for example, a corruption of one DB entry may cause a large number of entries to
    // become unreadable or for the entire DB to become unopenable. Default: false
    bool error_checks;

    // Amount of data to build up in memory(backed by an unsorted log on disk) before converting to a sorted 
    // on-disk file.
    // Larger values increase performance, especially during bulk loads. Up to two write buffers may be held 
    // in memory at the same time, so you may wish to adjust this parameter to control memory usage.
    // Also, a larger write buffer will result in a longer recovery time the next time the database is opened.
    // Default: 4MB
    size_t wirte_buffer_size;

    // Number of open files that can be used by the DB.  You may need to increase this if your database has a 
    // large working set (budget one open file per 2MB of working set). Default: 1000
    int max_open_files;

    //Cache* block_cache;

    // Approximate size of user data packed per block.
    size_t block_size;

    int block_restart_interval;

    size_t max_file_size;

    compressionType compression;


    //const FilterPolicy* filter_policy;

    Options();
};

struct ReadOptions {
    bool fill_cache;
    bool verify_checks;
    //const Snapshot* snapshot;
    ReadOptions() 
            : fill_cache(true), 
              verify_checks(false)
              //snapshot(NULL) 
              { }
};

struct WriteOptions {
    bool sync;

    // sequence number ------ global var
    uint64_t g_sequence_number;

    WriteOptions() : sync(false), g_sequence_number(0) { }
};


}


#endif