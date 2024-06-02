#include "sfs/FileSystemManager.h"
#include "sfs/fs.h"
#include <cstdio>
#include<string.h>
#include<string>
#include <unistd.h>
#include<iostream>
#include<algorithm>
#include <limits>
#include <cstdlib>

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
    DirItem item = makeDirItem(DIR, initRootDirSize, inumber, RWX, ".");
    appendDirItem(&data, item);
    // 准备一个DirItem文件名为：".."指向自己
    item = makeDirItem(DIR, initRootDirSize, inumber, RWX, "..");
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

void FileSystemManager::writeBackDir(int inumber, std::vector<DirItem>& items) {
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
    writeBackDir(rootDirInumber_, rootItems);
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

bool FileSystemManager::isFileExist(std::vector<DirItem>& items, const char* name) {
    for (const auto& item : items) {
        if (strcmp(item.FileName, name) == 0) {
            return true;
        }
    }
    return false;
}

void FileSystemManager::makeDir(int inumber, const char* name) {
    // 首先，创建/home目录这个事件，应该是在根目录/下发生的，所以根目录首先需要进行改变
    int newDirInumber = fileSystem_.create();
    auto parentItems = getDirItems(inumber);
    // 看看有没有同名的文件
    if (isFileExist(parentItems, name)) {
        std::string err;
        err = err + "The file: <" + name + "> is existing!\n";
        throw std::runtime_error(err.c_str());
    }
    // 在父目录下创建新目录
    DirItem item;
    item = makeDirItem(DIR, 2 * sizeof(DirItem), newDirInumber, RWX, name);
    parentItems.emplace_back(item);
    writeBackDir(inumber, parentItems);
    // 根目录的信息更新完毕，准备/home目录的基本信息
    // "."应指向/home自己
    // ".."应指向其parent目录，也就是根目录
    char* data = (char*)malloc(2 * sizeof(DirItem));
    char* start = data;
    if (data == NULL) {
        throw std::runtime_error("malloc() fail!\n");
    }
    item = makeDirItem(DIR, 2 * sizeof(DirItem), newDirInumber, RWX, ".");
    appendDirItem(&data, item);
    item = makeDirItem(DIR, fileSystem_.stat(inumber), inumber, RWX, "..");
    appendDirItem(&data, item);
    fileSystem_.write(newDirInumber, start, 2 * sizeof(DirItem), 0);
    free(start);
    /*
     *如果创建失败，那就需要保证根目录原有的数据不改变.这里简单起见先不考虑操作的原子性问题
     */
}

void FileSystemManager::registerUser(const char* username) {
    makeDir(homeDirInumber_, username);
}

void FileSystemManager::unregisterUser(const char* username) {
    auto items = getDirItems(homeDirInumber_);
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (strcmp(it->FileName, username) == 0) {
            if (it->FileType == DIR) {
                items.erase(it);
                writeBackDir(homeDirInumber_, items);
                return;
            }
            else {
                break;
            }
        }
    }
    std::string err;
    err = err + "The user: <" + username + "> isn't existing!\n";
    throw std::runtime_error(err.c_str());
}

void FileSystemManager::login(const char* username) {
    auto items = getDirItems(homeDirInumber_);
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (strcmp(it->FileName, username) == 0) {
            if (it->FileType == DIR) {
                workDirInumber_ = it->Inumber;
                return;
            }
            else {
                std::string err;
                err = err + "The user: <" + username + "> isn't existing!\n";
                throw std::runtime_error(err.c_str());
            }
        }
    }
}

void FileSystemManager::ls() {
    USING();
    auto items = getDirItems(workDirInumber_);
    int maxSpace = 0;
    for (const auto& item : items) {
        int size = std::to_string(fileSystem_.stat(item.Inumber)).size();
        if (size > maxSpace) {
            maxSpace = size;
        }
    }
    maxSpace += 2;
    for (const auto& item : items) {
        int FileSize = fileSystem_.stat(item.Inumber);
        int size = std::to_string(FileSize).size();
        std::string space;
        for (int i = 0; i < (maxSpace - size); i++) {
            space.append(" ");
        }
        cout << permissionToString(item.FilePermission, item.FileType) << "  "
             << FileSize << space.c_str()
             << item.FileName << std::endl;
    }
}

std::string FileSystemManager::permissionToString(uint32_t p, uint32_t type) {
    std::string str;
    switch(type) {
        case DIR:
            str += "d";
            break;
        case NORMAL:
            str += "-";
            break;
        default:
            break;
    }
    switch(p) {
        case NUL:
            str += "---";
            break;
        case X:
            str += "--x";
            break;
        case W:
            str += "-w-";
            break;
        case WX:
            str += "-wx";
        case R:
            str += "r--";
            break;
        case RX:
            str += "r-x";
            break;
        case RW:
            str += "rw-";
            break;
        case RWX:
            str += "rwx";
            break;
        default:
            break;
    }
    return str;
}

void FileSystemManager::mkdir(const char* filename) {
    makeDir(workDirInumber_, filename);
}

void FileSystemManager::cd(const char* dirname) {
    auto items = getDirItems(workDirInumber_);
    for (const auto& item : items) {
        if (strcmp(dirname, item.FileName) == 0) {
            if (item.FileType == DIR) {
                workDirInumber_ = item.Inumber;
            }
            else {
                std::string err;
                err = err + "The file <" + dirname + "> isn't a directory!\n";
                throw std::invalid_argument(err.c_str());
            }
        }
    }
}

std::string FileSystemManager::getDirName(int parentInumber, int childInumber) {
    auto items = getDirItems(parentInumber);
    for (const auto& item : items) {
        if (item.Inumber == childInumber) {
            return std::string(item.FileName);
        }
    }
}

void FileSystemManager::pwd() {
    int currentDir = workDirInumber_;
    std::vector<std::string> names;
    while (currentDir != rootDirInumber_) {
        auto items = getDirItems(currentDir);
        int dirInumber = -1;
        for (const auto& item : items) {
            if (strcmp(item.FileName, ".") == 0) {
                dirInumber = item.Inumber;
            }
            // 目录本身是不知道自己的名字的，目录的名字存储在parent目录中
            if (strcmp(item.FileName, "..") == 0) {
                names.emplace_back(getDirName(item.Inumber, dirInumber));
                currentDir = item.Inumber;
            }
        }
    }
    std::reverse(names.begin(), names.end());
    std::cout << "/";
    for (int i = 0 ; i < names.size(); i++) {
        std::cout << names[i];
        if (i < names.size() - 1) {
            std::cout << "/";
        }
    }
    std::cout << std::endl;
}

void FileSystemManager::touch(const char* filename) {
    auto items = getDirItems(workDirInumber_);
    // 先看看当前目录下有没有这个文件吧
    if (isFileExist(items, filename)) {
        std::string err;
        err = err + "The file <" + filename + "> is existing!\n";
        throw std::runtime_error(err.c_str());
    }
    int inumber = fileSystem_.create();
    DirItem item = makeDirItem(NORMAL, 0, inumber, RW, filename);
    items.emplace_back(item);
    writeBackDir(workDirInumber_, items);
}

bool FileSystemManager::haveWritePermission(uint32_t permission) {
    return permission == W || permission == RW || permission == WX || permission == RWX;
}

void FileSystemManager::rm(const char* filename) {
    auto items = getDirItems(workDirInumber_);
    if (strcmp(filename, ".") == 0 || strcmp(filename, "..")) {
        std::cerr << "System error! Can't remove this directory!" << std::endl;
    }
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (strcmp(filename, it->FileName) == 0) {
            // 有没有写权限？
            if (!haveWritePermission(it->FilePermission)) {
                std::string err;
                err = err + "Permission refused: can't remove <" + filename + ">\n";
                throw std::runtime_error(err.c_str());
            }
            // 有写权限自然是可以删除
            // 不过如果删除的文件是目录文件，就需要提醒一下
            if (it->FileType == DIR) {
                std::string answer;
                std::cout << "The file <"
                          << filename
                          <<"> is a directory, it will also remove all its sub-directory" << std::endl;
                std::cout << "Do you really want to remove it?(Y/N): ";
                std::cin >> answer;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (answer == "Y" || answer == "y") {
                    // 对应的Inode数据已经灰飞烟灭了
                    fileSystem_.remove(it->Inumber);
                    // 接下来就是将目录项中的有关该文件的信息清除
                    items.erase(it);
                }
                else {
                    return;
                }
            }
            else {
               fileSystem_.remove(it->Inumber) ;
                items.erase(it);
            }
            writeBackDir(workDirInumber_, items);
            return;
        }
    }
}

void FileSystemManager::vim(const char* filename) {
    // 看看这个文件存不存在
    auto items = getDirItems(workDirInumber_);
    int inumber = -1;
    for (auto& item : items) {
        if (strcmp(item.FileName, filename) == 0) {
            if (item.FileType == DIR) {
                throw std::runtime_error("Can't write a directory!\n");
            }
            if (!haveWritePermission(item.FilePermission)) {
                std::string err;
                err = err + "Permission refused: can't write <" + filename + ">\n";
                throw std::runtime_error(err.c_str());
            }
            inumber = item.Inumber;
        }
    }
    char str[BUFSIZ];
    std::string text;
    while(true) {
        fgets(str, BUFSIZ, stdin);
        if(strncmp(str, "exit", 4) == 0) {
            break;
        }
        text.append(str);
        memset(str, 0, BUFSIZ);
    }
    if (inumber == -1) {
        touch(filename);
        items = getDirItems(workDirInumber_);
        // 这里其实可以直接找最后一个元素，因为是刚插入的新文件
        for (auto& item : items) {
            if (strcmp(item.FileName, filename) == 0) {
                inumber = item.Inumber;
                break;
            }
        }
    }
    char* data = (char*)malloc(text.size());
    if (data == NULL) {
        throw std::runtime_error("malloc() failed!\n");
    }
    try {
        // 在这里读取的时候完全是没有问题的。
        memcpy(data, text.c_str(), text.size());
        fileSystem_.write(inumber, data, text.size(), 0);
        free(data);
    }
    catch(const std::exception& e) {
        throw e;
    }
}

void FileSystemManager::cat(const char* filename) {
    // 首先要找到对应的filename, 从目录中找filename这个操作完全可以独立成一个方法
    auto items = getDirItems(workDirInumber_);
    for (const auto& item : items) {
        if (strcmp(filename, item.FileName) == 0) {
            if (item.FileType == DIR) {
                std::string err;
                err = err + "cat: " + filename + ": Is a directory\n";
                throw std::runtime_error(err);
            }
            char* data = (char*)malloc(fileSystem_.stat(item.Inumber));
            if (data == NULL) {
                throw std::runtime_error("malloc() failed!\n");
            }
            try {
                fileSystem_.read(item.Inumber, data, fileSystem_.stat(item.Inumber), 0);
                std::cout << data << std::endl;
                free(data);
            }
            catch(const std::exception& e) {
                free(data);
                throw e;
            }
        }
    }
}
