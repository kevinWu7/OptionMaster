cmake_minimum_required (VERSION 3.1) #最低要求的CMake版本
project(cmake_demo) # 项目名称

file(GLOB SRC_FILE main.cpp) # 建立变量SRC_FILE为目录下.c文件列表

set(CMAKE_CXX_STANDARD 17) # 指定为C++11 版本
add_executable (${PROJECT_NAME} ${SRC_FILE}) # 要求编译可执行文件 