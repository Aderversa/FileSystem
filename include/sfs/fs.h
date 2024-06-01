// fs.h: File System

#pragma once

#include "sfs/disk.h"

#include <stdint.h>
#include <vector>

class FileSystem {
public:
    constexpr static uint32_t MAGIC_NUMBER	     = 0xf0f03410;
    constexpr static uint32_t INODES_PER_BLOCK   = 128;
	constexpr static uint32_t POINTERS_PER_INODE = 5;
	constexpr static uint32_t POINTERS_PER_BLOCK = 1024;
    constexpr static uint32_t FILENAME_MAX_LENGTH = 16; // 说是16实际上要存'\0'，所以只剩15个字符
    constexpr static uint32_t ITEMS_PER_BLOCK = 128; // 一个DirItem大小为32B，而一个块有4KB，所以最多能装128个Item
private:
    struct SuperBlock {		// Superblock structure
    	uint32_t MagicNumber;	// File system magic number
    	uint32_t Blocks;	// Number of blocks in file system
    	uint32_t InodeBlocks;	// Number of blocks reserved for inodes
    	uint32_t Inodes;	// Number of inodes in file system
    };
    struct Inode {
    	uint32_t Valid;		// Whether or not inode is valid
    	uint32_t Size;		// Size of file
    	uint32_t Direct[POINTERS_PER_INODE]; // Direct pointers
    	uint32_t Indirect;	// Indirect pointer
    };
	// Block有不同的类型，所以干脆写成联合体方便复用
    union Block {
    	SuperBlock  Super;			    // Superblock
    	Inode	    Inodes[INODES_PER_BLOCK];	    // Inode block
    	uint32_t    Pointers[POINTERS_PER_BLOCK];   // Pointer block
    	char	    Data[Disk::BLOCK_SIZE];	    // Data block
    };
    struct DirItem{
        uint32_t FileType;
        uint32_t FileSize;
        uint32_t FilePermission;
        char FileName[FILENAME_MAX_LENGTH];
    };
	size_t inodeBlocks = 0;
	// 需要记录空闲Block的位图
	std::vector<bool> freeDataBlockBitmap;
	// 需要记录空闲的Inode的位图
	std::vector<bool> freeInodeBitmap;
	Disk* m_disk = nullptr;
    // TODO: Internal helper functions
	ssize_t allocInode();
	ssize_t allocDataBlock();
	int getInodeDataBlock(Inode& inode, int nowBlock);
	void allocateNewBlock(Inode& inode);
    // TODO: Internal member variables
public:
    static void debug(Disk *disk);
    static bool format(Disk *disk);
    bool mount(Disk *disk);
	/*
	 *返回-1，表示分配失败；成功则返回分配的Inode号
	 */
    ssize_t create();
    bool    remove(size_t inumber);
    ssize_t stat(size_t inumber);
    ssize_t read(size_t inumber, char *data, size_t length, size_t offset);
    ssize_t write(size_t inumber, char *data, size_t length, size_t offset);
};
