cmake_minimum_required(VERSION 3.16)
project(oop_lab1)

set(CMAKE_CXX_STANDARD 20)

add_executable(oop_lab1
        main.cpp
        lru_cache.cpp
        lfu_cache.cpp
        fib_runner.cpp
        fib.cpp
)

target_include_directories(oop_lab1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests
        tests.cpp
        lru_cache.cpp
        lfu_cache.cpp
        fib_runner.cpp
        fib.cpp
)

target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(tests
        GTest::gtest
        GTest::gtest_main
)

add_test(NAME CacheTests COMMAND tests)
