cmake_minimum_required(VERSION 3.1)
project(test)

file(GLOB SRC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
#指定头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
#指定库文件路径
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs)

add_executable(${PROJECT_NAME} ${SRC_FILE})

#链接SniperSDK这个第三方库
target_link_libraries(${PROJECT_NAME} calc)

#安装-将生成的目标输出到指定目录（包含可执行文件和库文件）
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build/bin)

file(GLOB DLL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/libs/*.dll")
install(FILES  ${DLL_FILES} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build/bin)
