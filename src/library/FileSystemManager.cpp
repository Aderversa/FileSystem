#include "sfs/FileSystemManager.h"
#include "sfs/fs.h"
#include<string.h>
#include<string>
#include<iostream>

#define USING() \
    using std::cerr; \
    using std::cout; \
    using std::endl; \
    using std::cin; \


FileSystemManager::FileSystemManager(const std::string& diskFile, int blocks) {
    USING();
    // 先打开一个磁盘
    disk_ = std::make_unique<Disk>();
    disk_->open(diskFile.c_str(), blocks);
    // 挂载磁盘到文件系统上，让文件系统来管理磁盘, 如果磁盘本身就是合法的，那自然挂载成功
    if(!fileSystem_.mount(disk_.get())) {
        // 挂载失败的原因: MagicNumber不合法, 这通常是因为没有对磁盘进行格式化导致的
        // 需要提示用户是否需要进行格式化
        std::string answer;
        cout << "The Disk hasn't formated, do you want to format it?" << endl;
        cout << "(Y/N): ";
        cin  >> answer;
        if (answer == "Y" || answer == "y") {
            FileSystem::format(disk_.get());
            if(fileSystem_.mount(disk_.get())) {
                // 同意格式化之后，还需要格式化好文件目录，主要是UFD和MFD
                // 对于MFD，规定所有在/home目录下的所有目录，都是用户目录
                // 每当系统创建一个新用户，就会有一个对应的目录在/home下被创建
                // 每当系统注销一个用户，就会删除/home目录下所对应的目录
                createRootDir();
                createHomeDir();
            }
        }
    }
    // 默认将根目录存放在Inode号为0的文件中。如果不是这样，那出现的问题就是格式问题，不是本系统负责的内容
    getHomeDirInumber();
}

void FileSystemManager::createRootDir() {
    int inumber = fileSystem_.create();
    int initRootDirSize = 2 * sizeof(DirItem);
    char* data = (char*)malloc(initRootDirSize);
    char* start = data;
    if(data == NULL) {
        throw std::runtime_error("malloc() fail!\n");
    }
    // 准备一个DirItem文件名为："."指向自己
    DirItem item = makeDirItem(DIR, initRootDirSize, RWX, inumber, ".");
    appendDirItem(&data, item);
    // 准备一个DirItem文件名为：".."指向自己
    item = makeDirItem(DIR, initRootDirSize, RWX, inumber, "..");
    appendDirItem(&data, item);
    // 最后，将这个数据写入文件中
    fileSystem_.write(inumber, start, initRootDirSize, 0);
    rootDirInumber_ = inumber;
    free(start);
}

void FileSystemManager::appendDirItem(char** data, DirItem& item) {
    int size = sizeof(DirItem);
    memcpy(*data, (const void*)&item, size);
    *data += size;
}

FileSystemManager::DirItem FileSystemManager::makeDirItem(
    const uint32_t& fileType,
    const uint32_t& fileSize,
    const uint32_t& inumber,
    const uint32_t& filePermission,
    const char* filename) {
    DirItem item;
    item.FileType = fileType;
    item.FileSize = fileSize;
    item.FilePermission = filePermission;
    item.Inumber = inumber;
    memset(item.FileName, 0, FILENAME_MAX_LENGTH);
    int filenameLen = (strlen(filename) + 1) >= FILENAME_MAX_LENGTH ? FILENAME_MAX_LENGTH : (strlen(filename) + 1);
    memcpy(item.FileName, filename, filenameLen);
    return item;
}

std::vector<FileSystemManager::DirItem> FileSystemManager::getDirItems(int inumber) {
    // 第一步应该查看目录的大小，以此来判断有多少个目录项
    int size = fileSystem_.stat(inumber);
    int itemNum = size / (sizeof(DirItem));
    // 获取完成之后，就是真正开始从磁盘中读取存储目录项的数据块
    char* data = (char*)malloc(size);
    char* start = data;
    if (data == NULL) {
        throw std::runtime_error("malloc() fail!\n");
    }
    fileSystem_.read(inumber, data, size, 0);
    // 从获取的数据中, 读取目录项数据
    std::vector<DirItem> items;
    for (int i = 0; i < itemNum; i++) {
        DirItem item;
        memcpy(&item, data, sizeof(DirItem));
        data = data + sizeof(DirItem);
        items.emplace_back(item);
    }
    free(start);
    return items;
}

void FileSystemManager::writeBackDir(int inumber, std::vector<DirItem> items) {
    char* data = (char*)malloc(items.size() * sizeof(DirItem));
    char* start = data;
    if (data == NULL) {
        throw std::runtime_error("malloc() fail!\n");
    }
    for (int i = 0; i < items.size(); i++) {
        memcpy(data, &items[i], sizeof(DirItem));
        data += sizeof(DirItem);
    }
    fileSystem_.write(inumber, start, items.size() * sizeof(DirItem), 0);
    free(start);
}

void FileSystemManager::createHomeDir() {
    // 首先，创建/home目录这个事件，应该是在根目录/下发生的，所以根目录首先需要进行改变
    int homeInumber = fileSystem_.create();
    auto rootItems = getDirItems(rootDirInumber_);
    DirItem item;
    item = makeDirItem(DIR, 2 * sizeof(DirItem), homeInumber, RWX, "home");
    rootItems.emplace_back(item);
    writeBackDir(rootDirInumber_, std::move(rootItems));
    // 根目录的信息更新完毕，准备/home目录的基本信息
    // "."应指向/home自己
    // ".."应指向其parent目录，也就是根目录
    char* data = (char*)malloc(2 * sizeof(DirItem));
    char* start = data;
    if (data == NULL) {
        throw std::runtime_error("malloc() fail!\n");
    }
    item = makeDirItem(DIR, 2 * sizeof(DirItem), homeInumber, RWX, ".");
    appendDirItem(&data, item);
    item = makeDirItem(DIR, fileSystem_.stat(rootDirInumber_), rootDirInumber_, RWX, "..");
    appendDirItem(&data, item);
    fileSystem_.write(homeInumber, start, 2 * sizeof(DirItem), 0);
    homeDirInumber_ = homeInumber;
    free(start);
    /*
     *如果创建失败，那就需要保证根目录原有的数据不改变.这里简单起见先不考虑操作的原子性问题
     */
}

void FileSystemManager::getHomeDirInumber() {
    auto items = getDirItems(rootDirInumber_);
    for(const auto& item : items) {
        if (strcmp("home", item.FileName) == 0) {
            homeDirInumber_ = item.Inumber;
        }
    }
}
