//
// Created by 24576 on 2024/5/31.
//
#include <sfs/FileSystemManager.h>
#include <string>
int main() {
    std::string diskname("../data/image.20");
    FileSystemManager file_system_manager(diskname, 20);
    return 0;
}
