// fs.cpp: File System

#include "sfs/fs.h"

#include <algorithm>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

// Debug file system -----------------------------------------------------------

void FileSystem::debug(Disk *disk) {
    Block block;
    int i = 0;
    // Read Superblock
    disk->read(0, block.Data);

    printf("SuperBlock:\n");
    printf("    %u blocks\n"         , block.Super.Blocks);
    printf("    %u inode blocks\n"   , block.Super.InodeBlocks);
    printf("    %u inodes\n"         , block.Super.Inodes);
    // Read Inode blocks, 将所有非空闲的Inode的状态都显示出来
    size_t n = block.Super.InodeBlocks;
    /*
     *因为Inode Block是从第一个块开始的
     */
    for(i = 1; i <= n; i++) {
        disk->read(i, block.Data);
        // 对于每一个Inode块，都有INODES_PER_BLOCK个Inode
        for(int j = 0; j < INODES_PER_BLOCK; j++) {
            // 将非空闲的Inode打印出来
            const Inode& inode = block.Inodes[j];
            if(inode.Valid == 0) {
                continue;
            }
            printf("Inode %d:\n", (i - 1) * INODES_PER_BLOCK + j);
            printf("    size: %d bytes\n", inode.Size);
            int directNums = 0;
            for (int k = 0; k < POINTERS_PER_INODE; k++) {
                if(inode.Direct[k] != 0) {
                    directNums++;
                }
            }
            printf("    direct blocks: %d\n", directNums);
            printf("    indirect blocks: %d\n", inode.Indirect == 0 ? 0 : 1);
        }
    }

}

// Format file system ----------------------------------------------------------

bool FileSystem::format(Disk *disk) {
    // Write superblock
    Block block;
    block.Super.MagicNumber = MAGIC_NUMBER;
    block.Super.Blocks = disk->size();
    block.Super.InodeBlocks = block.Super.Blocks / 10;
    block.Super.Inodes = block.Super.InodeBlocks * INODES_PER_BLOCK;
    disk->write(0, block.Data);
    // Clear all other blocks
    memset((void*)block.Data, 0, Disk::BLOCK_SIZE) ;
    for (int i = 1; i < disk->size(); i++) {
        disk->write(i, block.Data);
    }
    return true;
}

// Mount file system -----------------------------------------------------------

bool FileSystem::mount(Disk *disk) {
    Block block;
    // Read superblock
    disk->read(0, block.Data) ;
    if (block.Super.MagicNumber != MAGIC_NUMBER) {
        return false;
    }
    // Set device and mount
    disk->mount();
    m_disk = disk;
    // Copy metadata
    inodeBlocks = block.Super.InodeBlocks;
    // Allocate free block bitmap
    // 遍历Inode，将空闲的Inode加入freeInodeBitmap
    freeInodeBitmap.clear();
    freeInodeBitmap.resize(INODES_PER_BLOCK * inodeBlocks, true);
    freeDataBlockBitmap.clear();
    freeDataBlockBitmap.resize(disk->size() - inodeBlocks - 1, true);
    for(int i = 1; i <= inodeBlocks; i++) {
        disk->read(i, block.Data) ;
        // 对于每一个Inode块，都有INODES_PER_BLOCK个Inode
        for(int j = 0; j < INODES_PER_BLOCK; j++) {
            // 将非空闲的Inode打印出来
            const Inode& inode = block.Inodes[j];
            if(inode.Valid == 1) {
                freeInodeBitmap[(i - 1) * INODES_PER_BLOCK + j] = false;
            }
            // 由于这个Inode不为空，所以拥有DataBlock, 也有可能啥也没存这个Inode
            for (int k = 0; k < POINTERS_PER_INODE; k++) {
                if(inode.Direct[k] != 0) {
                    freeDataBlockBitmap[inode.Direct[k] - inodeBlocks - 1] = false;
                }
            }
            if (inode.Indirect != 0) {
                freeDataBlockBitmap[inode.Indirect - inodeBlocks - 1] = false;
            }
        }
    }
    return true;
}

// Create inode ----------------------------------------------------------------

ssize_t FileSystem::create() {
    // Locate free inode in inode table
    try {
        Block block;
        ssize_t inodeNumber = allocInode();
        if (inodeNumber < 0) {
            return -1;
        }
        // Record inode if found
        size_t inodeBlock = (inodeNumber / INODES_PER_BLOCK) + 1;
        size_t inodeInBlock = inodeNumber % INODES_PER_BLOCK;
        // 读出Inode所在的Inode块
        m_disk->read(inodeBlock, block.Data);
        // 初始化Inode节点，一开始的Inode当然是没有存东西，所以没有指向数据块
        Inode& inode = block.Inodes[inodeInBlock];
        inode.Valid = 1;
        inode.Size = 0;
        for (int i = 0; i < POINTERS_PER_INODE; i++) {
            inode.Direct[i] = 0;
        }
        inode.Indirect = 0;
        // 将修改的数据写回去
        m_disk->write(inodeBlock, block.Data);
        return static_cast<ssize_t>(inodeNumber);
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}

// Remove inode ----------------------------------------------------------------

bool FileSystem::remove(size_t inumber) {
    try {
        Block block;
        Block tmpBlock;
        size_t inodeBlock = inumber / INODES_PER_BLOCK + 1;
        size_t inodeInBlock = inumber % INODES_PER_BLOCK;
        // Load inode information
        m_disk->read(inodeBlock, block.Data);
        Inode& inode = block.Inodes[inodeInBlock];
        // Free direct blocks
        // 所谓释放数据块，其实就是操纵这些数据块的空闲位图
        for (int i = 0; i < POINTERS_PER_INODE; i++) {
            if (inode.Direct[i] >= (1 + inodeBlocks)) {
                freeDataBlockBitmap[inode.Direct[i] - 1 - inodeBlocks] = true;
                m_disk->read(inode.Direct[i], tmpBlock.Data) ;
                memset(tmpBlock.Data, 0, Disk::BLOCK_SIZE);
            }
            inode.Direct[i] = 0;
        }
        // Free indirect blocks
        if (inode.Indirect >= (1 + inodeBlocks)) {
            freeDataBlockBitmap[inode.Indirect - 1 - inodeBlocks] = true;
                m_disk->read(inode.Indirect, tmpBlock.Data) ;
                memset(tmpBlock.Data, 0, Disk::BLOCK_SIZE);
        }
        inode.Indirect = 0;
        // Clear inode in inode table
        inode.Valid = 0;
        inode.Size = 0;
        freeInodeBitmap[inumber] = true;
        // 回写到Disk
        m_disk->write(inodeBlock, block.Data);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

// Inode stat ------------------------------------------------------------------

ssize_t FileSystem::stat(size_t inumber) {
    // Load inode information
    try {
        if (freeInodeBitmap[inumber]) {
            return -1;
        }
        size_t inodeBlock = (inumber / INODES_PER_BLOCK) + 1;
        size_t inodeInBlock = inumber % INODES_PER_BLOCK;
        Block block;
        m_disk->read(inodeBlock, block.Data);
        return block.Inodes[inodeInBlock].Size;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}

// Read from inode -------------------------------------------------------------

ssize_t FileSystem::read(size_t inumber, char *data, size_t length, size_t offset) {
    Block block;
    Block dataBlock;
    char* start = data;
    m_disk->read(inumber / Disk::BLOCK_SIZE + 1, block.Data);
    Inode& inode = block.Inodes[inumber % Disk::BLOCK_SIZE];
    if(offset > inode.Size) {
        return -1;
    }
    if(offset + length > inode.Size) {
        length = inode.Size - offset;
    }
    int beginOffset = offset % Disk::BLOCK_SIZE;
    int beginDataBlock = offset / Disk::BLOCK_SIZE;
    int endDataBlock = (offset + length) / Disk::BLOCK_SIZE;
    int remainRead = (offset + length) % Disk::BLOCK_SIZE;
    int readNum = 0;
    if (beginOffset + length < Disk::BLOCK_SIZE) {
        int dataBlockNum = getInodeDataBlock(inode, beginDataBlock);
        m_disk->read(dataBlockNum, dataBlock.Data);
        memcpy(data, dataBlock.Data + beginOffset, length);
        data += length;
        readNum += length;
    }
    else {
        for (int i = beginDataBlock; i <= endDataBlock; i++) {
            int dataBlockNum = getInodeDataBlock(inode, beginDataBlock);
            m_disk->read(dataBlockNum, dataBlock.Data);
            if (i == beginDataBlock) {
                m_disk->read(dataBlockNum, dataBlock.Data);
                memcpy(data, dataBlock.Data + beginOffset, Disk::BLOCK_SIZE - beginOffset);
                data += Disk::BLOCK_SIZE - beginOffset;
                readNum += Disk::BLOCK_SIZE - beginOffset;
            }
            else if (i == endDataBlock) {
                m_disk->read(dataBlockNum, dataBlock.Data);
                memcpy(data, dataBlock.Data, remainRead);
                data += remainRead;
                readNum += remainRead;
            }
            else {
                m_disk->read(dataBlockNum, dataBlock.Data);
                memcpy(data, dataBlock.Data, Disk::BLOCK_SIZE);
                data += Disk::BLOCK_SIZE;
                readNum += Disk::BLOCK_SIZE;
            }
        }
    }
    data = start;
    return readNum;
}

// Write to inode --------------------------------------------------------------

ssize_t FileSystem::write(size_t inumber, char *data, size_t length, size_t offset) {
    size_t MAX_LENGTH = 33685504; // 16KB + 16MB
    Block block;
    if (offset + length > MAX_LENGTH) {
        length = MAX_LENGTH - offset;
    }
    m_disk->read(inumber / Disk::BLOCK_SIZE + 1, block.Data);
    Inode& inode = block.Inodes[inumber % Disk::BLOCK_SIZE];
    if (offset > inode.Size) {
        return -1;
    }
    // 要去找一个数据块出来
    // 数据块需要通过offset来获得
    int beginDataBlock = offset / Disk::BLOCK_SIZE;
    int beginOffset = offset % Disk::BLOCK_SIZE;
    int endDataBlock = (offset + length) / Disk::BLOCK_SIZE;
    int remianWrite = (offset + length) % Disk::BLOCK_SIZE;
    inode.Size = offset + length;
    int writeNum = 0;
    if (beginOffset + length < Disk::BLOCK_SIZE) {
        Block dataBlock;
        int dataBlockNum = getInodeDataBlock(inode, beginDataBlock);
        if (dataBlockNum <= 0) {
            allocateNewBlock(inode);
            dataBlockNum = getInodeDataBlock(inode, beginDataBlock);
        }
        m_disk->read(dataBlockNum, dataBlock.Data);
        memcpy(dataBlock.Data, data, length);
        m_disk->write(dataBlockNum, dataBlock.Data);
        writeNum += length;
    }
    else {
        for (int i = beginDataBlock; i <= endDataBlock; i++) {
            Block dataBlock;
            int dataBlockNum = getInodeDataBlock(inode, i);
            // 但是如果数据块是空的，就需要进行分配
            if (dataBlockNum <= 0) {
                allocateNewBlock(inode);
                i--;
                continue;
            }
            if (i == beginDataBlock) {
                m_disk->read(dataBlockNum, dataBlock.Data);
                memcpy(dataBlock.Data, data, Disk::BLOCK_SIZE - beginOffset);
                data = data + Disk::BLOCK_SIZE - beginOffset;
                m_disk->write(dataBlockNum, dataBlock.Data);
                writeNum += (Disk::BLOCK_SIZE - beginOffset);
            }
            else if (i == endDataBlock) {
                m_disk->read(dataBlockNum, dataBlock.Data) ;
                memcpy(dataBlock.Data, data, remianWrite) ;
                data = data + remianWrite;
                m_disk->write(dataBlockNum, dataBlock.Data);
                writeNum += remianWrite;
            }
            else {
                m_disk->read(dataBlockNum, dataBlock.Data) ;
                memcpy(dataBlock.Data, data, Disk::BLOCK_SIZE) ;
                data = data + Disk::BLOCK_SIZE;
                m_disk->write(dataBlockNum, dataBlock.Data);
                writeNum += Disk::BLOCK_SIZE;
            }

        }
    }
    m_disk->write(inumber / Disk::BLOCK_SIZE + 1, block.Data);
    return writeNum;
}

// find free inode --------------------------------------------------------------

ssize_t FileSystem::allocInode() {
    size_t i = 0;
    for (i = 0; i < freeInodeBitmap.size(); i++) {
        if(freeInodeBitmap[i]) {
            freeInodeBitmap[i] = false;
            return i;
        }
    }
    // 找不到空闲的Inode
    return -1;
}

// find free data block --------------------------------------------------------------

ssize_t FileSystem::allocDataBlock() {
    size_t i = 0;
    for (i = 0; i < freeDataBlockBitmap.size(); i++) {
        if (freeDataBlockBitmap[i]) {
            freeDataBlockBitmap[i] = false;
            return i + 1 + inodeBlocks;
        }
    }
    return -1;
}

/*
 *直接返回直接指针
 *如果nowBlock在间接指针块中，然而间接指针块不存在，返回-1
 */
int FileSystem::getInodeDataBlock(Inode& inode, int nowBlock) {
    if (nowBlock < POINTERS_PER_INODE) {
        return inode.Direct[nowBlock];
    }
    else {
        Block indirectBlock;
        if (inode.Indirect == 0) {
            return  -1;
        }
        else {
            m_disk->read(inode.Indirect, indirectBlock.Data);
            nowBlock -= POINTERS_PER_INODE;
            return indirectBlock.Pointers[nowBlock];
        }
    }
}

void FileSystem::allocateNewBlock(Inode& inode) {
    for(int i = 0; i < POINTERS_PER_INODE; i++) {
        if (inode.Direct[i] == 0) {
            int newDataBlock = allocDataBlock();
            if (newDataBlock < 0) {
                throw std::runtime_error("disk is full");
            }
            inode.Direct[i] = newDataBlock;
            return;
        }
    }
    if (inode.Indirect == 0) {
            int newDataBlock = allocDataBlock();
            if (newDataBlock < 0) {
                throw std::runtime_error("disk is full");
            }
            inode.Indirect = newDataBlock;
    }
    Block indirectBlock;
    m_disk->read(inode.Indirect, indirectBlock.Data);
    int newDataBlock = allocDataBlock();
    if (newDataBlock < 0) {
        throw std::runtime_error("disk is full");
    }
    for (int i = 0; i < POINTERS_PER_BLOCK; i++) {
        if (indirectBlock.Pointers[i] == 0) {
            int newDataBlock = allocDataBlock();
            if (newDataBlock < 0) {
                throw std::runtime_error("disk is full");
            }
            indirectBlock.Pointers[i] = newDataBlock;
            m_disk->write(inode.Indirect, indirectBlock.Data);
            return;
        }
    }
    throw std::runtime_error("file is full");
}
