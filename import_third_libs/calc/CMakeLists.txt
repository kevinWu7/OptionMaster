cmake_minimum_required(VERSION 3.1)
project(calc)
file(GLOB SRC_FILE "${CMAKE_SOURCE_DIR}/calc.cpp")

add_library(${PROJECT_NAME} STATIC ${SRC_FILE})
#add_library(${PROJECT_NAME} SHARED ${SRC_FILE})
#add_executable(${PROJECT_NAME}  ${SRC_FILE})


