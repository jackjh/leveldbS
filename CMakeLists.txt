cmake_minimum_required(VERSION 3.9)
project(leveldbS VERSION 1.0 LANGUAGES C CXX)

#This project can use C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS OFF)

# This project requires C++11.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(LEVELDB_BUILD_TESTS "Build LevelDB's unit tests" ON)
option(LEVELDB_BUILD_BENCHMARKS "Build LevelDB's benchmarks" ON)
option(LEVELDB_INSTALL "Install LevelDB's header and library" ON)

include(TestBigEndian)
test_big_endian(LEVELDB_IS_BIG_ENDIAN)

include(CheckIncludeFile)
check_include_file("unistd.h" HAVE_UNISTD_H)

include(CheckLibraryExists)
check_library_exists(crc32c crc32c_value "" HAVE_CRC32C)
check_library_exists(snappy snappy_compress "" HAVE_SNAPPY)
check_library_exists(tcmalloc malloc "" HAVE_TCMALLOC)

include(CheckSymbolExists)
check_symbol_exists(fdatasync "unistd.h" HAVE_FDATASYNC)

include(CheckCXXSourceCompiles)


set(OLD_CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS})
list(APPEND CMAKE_REQUIRED_FLAGS -Werror -Wthread-safety)
check_cxx_source_compiles("
struct __attribute__((lockable)) Lock {
  void Acquire() __attribute__((exclusive_lock_function()));
  void Release() __attribute__((unlock_function()));
};
struct ThreadSafeType {
  Lock lock_;
  int data_ __attribute__((guarded_by(lock_)));
};
int main() { return 0; }
"  HAVE_CLANG_THREAD_SAFETY)
set(CMAKE_REQUIRED_FLAGS ${OLD_CMAKE_REQUIRED_FLAGS})

# Test whether C++17 __has_include is available.
check_cxx_source_compiles("
#if defined(__has_include) &&  __has_include(<string>)
#include <string>
#endif
int main() { std::string str; return 0; }
" HAVE_CXX17_HAS_INCLUDE)


set(LEVELDB_PUBLIC_INCLUDE_DIR "include/leveldb")
set(LEVELDB_PORT_CONFIG_DIR "include/port")

configure_file(
  "${PROJECT_SOURCE_DIR}/port/port_config.h.in"
  "${PROJECT_BINARY_DIR}/${LEVELDB_PORT_CONFIG_DIR}/port_config.h"
)

include_directories(
  "${PROJECT_BINARY_DIR}/include"
  "${PROJECT_SOURCE_DIR}"
)

if(BUILD_SHARED_LIBS)
  # Only export LEVELDB_EXPORT symbols from the shared library.
  add_compile_options(-fvisibility=hidden)
endif(BUILD_SHARED_LIBS)

add_library(leveldb "")
target_sources(leveldb
    PRIVATE
        "${PROJECT_SOURCE_DIR}/db/db_imple.cc"
        "${PROJECT_SOURCE_DIR}/db/db_imple.h"
        "${PROJECT_SOURCE_DIR}/db/db_format.h"
        "${PROJECT_SOURCE_DIR}/db/version_edit.h"
        "${PROJECT_SOURCE_DIR}/db/version_set.h"
        "${PROJECT_SOURCE_DIR}/util/arena.cc"
        "${PROJECT_SOURCE_DIR}/util/arena.h"
        "${PROJECT_SOURCE_DIR}/util/random.h"
        "${PROJECT_SOURCE_DIR}/util/options.cc"
        "${PROJECT_SOURCE_DIR}/util/mutexlock.h"
        "${PROJECT_SOURCE_DIR}/port/port_stdcxx.h"

)


target_include_directories(leveldb
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)


if(LEVELDB_BUILD_TESTS)
  enable_testing()

  function(leveldb_test test_file)
    get_filename_component(test_target_name "${test_file}" NAME_WE)

    add_executable("${test_target_name}" "")
    target_sources("${test_target_name}"
      PRIVATE
        "${PROJECT_SOURCE_DIR}/util/testharness.cc"
        "${PROJECT_SOURCE_DIR}/util/testharness.h"

        "${test_file}"
    )
    target_link_libraries("${test_target_name}" leveldb)
    target_compile_definitions("${test_target_name}"
      PRIVATE
        LEVELDB_PLATFORM_POSIX=1
    )
    if (NOT HAVE_CXX17_HAS_INCLUDE)
      target_compile_definitions("${test_target_name}"
        PRIVATE
          LEVELDB_HAS_PORT_CONFIG_H=1
      )
    endif(NOT HAVE_CXX17_HAS_INCLUDE)
    
    add_test(NAME "${test_target_name}" COMMAND "${test_target_name}")
  endfunction(leveldb_test)
  
  if(NOT BUILD_SHARED_LIBS)
    leveldb_test("${PROJECT_SOURCE_DIR}/util/arena_test.cc")
    leveldb_test("${PROJECT_SOURCE_DIR}/db/db_imple_test.cc")
  endif(NOT BUILD_SHARED_LIBS)

endif(LEVELDB_BUILD_TESTS)