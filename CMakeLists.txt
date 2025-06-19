cmake_minimum_required(VERSION 3.10)
project(AdvancedAllocator VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    src/allocator.cpp
    src/pool_allocator.cpp
    src/buddy_allocator.cpp
    internal/utils.cpp
)

add_library(advanced_allocator ${SOURCES})

option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

add_executable(allocator_demo src/main.cpp)
target_link_libraries(allocator_demo advanced_allocator) 