cmake_minimum_required(VERSION 3.0.0)
project(TinyWebServer VERSION 0.1.0)

# 指定为C++11 版本
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/http)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/threadpool)


# add_executable(threadpool ./threadpool/threadpool.cpp)

set(SRC_LIST ./threadpool/test_for_threadpool.cpp)
add_executable(test ${SRC_LIST})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

