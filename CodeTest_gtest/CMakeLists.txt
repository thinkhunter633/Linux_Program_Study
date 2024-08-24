# CMakeLists.txt  
cmake_minimum_required(VERSION 3.10) 
project(SumTest) 
 
# 添加gtest库 
find_package(GTest REQUIRED) 
include_directories(${GTEST_INCLUDE_DIRS}) 
 
# 添加源文件 
add_executable(SumTest test_sum.cpp  sum.c)  
 
# 链接gtest库 
target_link_libraries(SumTest ${GTEST_LIBRARIES} pthread) 

set(CMAKE_BUILD_TYPE Debug)