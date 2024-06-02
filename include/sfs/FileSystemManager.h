#pragma once

#include <sfs/fs.h>
#include <sfs/disk.h>
#include <memory>
#include <vector>

class FileSystemManager {
public:
    constexpr static uint32_t FILENAME_MAX_LENGTH = 16; // 说是16，但由于需要存'\0'所以实际是15
private:
    enum TYPE {
        DIR = 0,
        NORMAL = 1
    };
    enum PERMISSION {
        NUL = 0,
        X   = 1,
        W   = 2,
        WX  = 3,
        R   = 4,
        RX  = 5,
        RW  = 6,
        RWX = 7
    };
    struct DirItem {
        uint32_t FileType;
        uint32_t FileSize;
        uint32_t Inumber;
        uint32_t FilePermission;
        char FileName[FILENAME_MAX_LENGTH];
    };
public:
    FileSystemManager(const std::string& diskFile, int blocks);
    void registerUser(const char* username);
    void unregisterUser(const char* username);
    void login(const char* username);
    void ls();
    void mkdir(const char* filename);
    void cd(const char* dirname);
    void pwd();
    void touch(const char* filename);
    void rm(const char* filename);
    void vim(const char* filename);
    void cat(const char* filename);
private:
    void createRootDir();
    void createHomeDir();
    void makeDir(int inumber, const char* name);
    bool isFileExist(std::vector<DirItem>& items, const char* name);
    void getHomeDirInumber();
    void appendDirItem(char** data, DirItem& item);
    void writeBackDir(int inumber, std::vector<DirItem>& items);
    bool haveWritePermission(uint32_t permission);
    std::string getDirName(int parentInumber, int childInumber);
    std::string permissionToString(uint32_t p, uint32_t type);
    DirItem makeDirItem(const uint32_t& fileType, const uint32_t& fileSize, const uint32_t& inumber,
                        const uint32_t& filePermission, const char* filename);
    // 获取inumber的所有目录项, 默认程序通过某种方式已经判断了inumber对应的文件是一个目录文件
    std::vector<DirItem> getDirItems(int inumber);
    std::unique_ptr<Disk> disk_ = nullptr;
    FileSystem fileSystem_;
    int rootDirInumber_ = 0;
    int homeDirInumber_;
    int workDirInumber_;
};



