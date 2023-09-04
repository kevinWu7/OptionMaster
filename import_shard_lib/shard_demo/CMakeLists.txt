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

#将动态库也安装到指定目录
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin") 
    file(GLOB DLL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/libs/*.dylib")
    install(FILES  ${DLL_FILES} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build/bin/Frameworks)
    #mac下，对动态库路径检查非常严格，需设置运行时查找动态库的路径，否则会报找不到dylib
    set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "@executable_path/Frameworks/")
    set_target_properties(${PROJECT_NAME} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    file(GLOB DLL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/libs/*.dll")
    install(FILES  ${DLL_FILES} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/build/bin)
endif()