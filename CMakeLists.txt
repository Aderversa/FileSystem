cmake_minimum_required(VERSION 3.15)
project(FileSystemProject)

set(CMAKE_CXX_STANDARD 17)

include_directories("${CMAKE_SOURCE_DIR}/include")
add_subdirectory(src)

add_executable(FileSystemProject "src/shell/mybash.cpp")
target_link_libraries(FileSystemProject PUBLIC FileSystem Disk FileSystemManager)