cmake_minimum_required(VERSION 3.0.0)
project(TinyWebServer VERSION 0.1.0)

include(CTest)
enable_testing()

# # 设置指定的C++编译器版本是必须的，如果不设置，或者为OFF，则指定版本不可用时，会使用上一版本。
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # 指定为C++11 版本
# set(CMAKE_CXX_STANDARD 11)

add_executable(TinyWebServer main.cpp)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
