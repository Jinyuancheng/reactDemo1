﻿
#=======================================================================
#Cmake最低版本要求 
CMAKE_MINIMUM_REQUIRED(VERSION 3.8) 
#设置程序名称
set(CMAKE_PROCNAME test_gdb)
PROJECT(${CMAKE_PROCNAME}) #项目名称 
 
#允许gdb调试 
set(CMAKE_BUILD_TYPE "Debug") 
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb") 
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall") 
 
#添加C++11支持及其他选项 
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs -Wno-deprecated") 
 
#=========================================================================
 
#添加所有头文件
include_directories(
	#"${CMAKE_CURRENT_SOURCE_DIR}/student"
	#"${CMAKE_CURRENT_SOURCE_DIR}/src"
)
 
#添加所有的源文件
file(GLOB all_src_file
	"${CMAKE_CURRENT_SOURCE_DIR}/*"
	#"${CMAKE_CURRENT_SOURCE_DIR}/student/*"
)
 
#添加动态库（路径）
link_directories(
	#"${CMAKE_CURRENT_SOURCE_DIR}/lib"
	"/usr/lib64/"
)
 
#=========================================================================
#生成可执行文件 
add_executable(${CMAKE_PROCNAME} ${all_src_file}) 
 
#添加链接库(具体是哪个动态库) 
target_link_libraries(${CMAKE_PROCNAME} 
	#rtmp
	#boost_thread
	#pthread
	*
)
