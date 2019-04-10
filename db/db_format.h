#ifndef LEVELDB_DB_DB_FORMAT_H_
#define LEVELDB_DB_DB_FORMAT_H_

namespace leveldb {

namespace config {

// leveldb中的层数
static const int kNumLevels = 7;

// 当level 0中达到该文件个数时，会触发level 0的compaction
static const int kL0CompactionTrigger = 4;

// 当level 0中达到该文件个数时，会减慢写入的速度
static const int KL0SlowdownWritesTrigger = 8;

// level 0最大的文件个数，当被触发时，停止写入
static const int KL0StopWritesTrigger = 12;

// 对memtable进行合并时，最多只能合并到level 2
static const int KMaxMemtableCompactLevel = 2;

}

}



#endif