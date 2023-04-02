cmake_minimum_required(VERSION 3.4) 
project(my_project) #设置项目名字

# GoogleTest requires at least C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall")  #设置常用的flag 这里-std=c++11采用C++11标准，
#-g支持gdb，-Wall打印所有信息

find_package(GTest REQUIRED) # 查找gtest包

include_directories(${GTEST_INCLUDE_DIRS}) #包括gtest头文件


add_executable(
  mytest #生成二进制文件名
  test.cc #源文件
)

target_link_libraries(mytest ${GTEST_BOTH_LIBRARIES})#添加链接库
enable_testing() #使能测试

add_test(
  Test  #测试名
  mytest
)#关键！添加测试

