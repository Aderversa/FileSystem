add_library(Disk disk.cpp)
target_include_directories(Disk PUBLIC "${CMAKE_SOURCE_DIR}/include")

add_library(FileSystem fs.cpp)
target_include_directories(FileSystem PUBLIC "${CMAKE_SOURCE_DIR}/include")

add_library(FileSystemManager FileSystemManager.cpp)
target_include_directories(FileSystemManager PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(FileSystemManager PUBLIC Disk FileSystem)
