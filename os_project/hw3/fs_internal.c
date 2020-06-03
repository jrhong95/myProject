#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disk.h"
#include "fs.h"

void FileSysInit(){
    int i;
    char* buf;
    Inode* inodeBuf;
    
    // Init FileSysInfo block
    buf = (char*)calloc(BLOCK_SIZE/sizeof(int), sizeof(int));
    DevWriteBlock(FILESYS_INFO_BLOCK, buf);

    // Init bytemap
    for(i = INODE_BYTEMAP_BLOCK_NUM; i <= BLOCK_BYTEMAP_BLOCK_NUM; i++){
        buf = (char*)calloc(BLOCK_SIZE, 1);
        DevWriteBlock(i, buf);
    }

    // Init Inode list
    for(i = INODELIST_BLOCK_FIRST; i < 7; i++){
        inodeBuf = (Inode*)calloc(BLOCK_SIZE/sizeof(Inode), sizeof(Inode));
        DevWriteBlock(i, (char*)inodeBuf);
    }

    // Init Data Region
    for(i = 7; i < FS_DISK_CAPACITY/BLOCK_SIZE; i++){
        buf = (char*)calloc(BLOCK_SIZE, 1);
        DevWriteBlock(i, buf);
    }

    //Init FileDesc table
    for(i = 0; i < MAX_FD_ENTRY_MAX; i++){
        pFileDesc[i].bUsed = 0;
        pFileDesc[i].pOpenFile = NULL;
    }
}

void SetInodeBytemap(int inodeno)
{
    char* buf = (char*)malloc(BLOCK_SIZE);

    DevReadBlock(INODE_BYTEMAP_BLOCK_NUM, buf);
    buf[inodeno] = 1;
    DevWriteBlock(INODE_BYTEMAP_BLOCK_NUM, buf);
}


void ResetInodeBytemap(int inodeno)
{
    char* buf = (char*)malloc(BLOCK_SIZE);

    DevReadBlock(INODE_BYTEMAP_BLOCK_NUM, buf);
    buf[inodeno] = 0;
    DevWriteBlock(INODE_BYTEMAP_BLOCK_NUM, buf);
}


void SetBlockBytemap(int blkno)
{
    char* buf = (char*)malloc(BLOCK_SIZE);

    DevReadBlock(BLOCK_BYTEMAP_BLOCK_NUM, buf);
    buf[blkno] = 1;
    DevWriteBlock(BLOCK_BYTEMAP_BLOCK_NUM, buf);
}


void ResetBlockBytemap(int blkno)
{
    char* buf = (char*)malloc(BLOCK_SIZE);

    DevReadBlock(BLOCK_BYTEMAP_BLOCK_NUM, buf);
    buf[blkno] = 0;
    DevWriteBlock(BLOCK_BYTEMAP_BLOCK_NUM, buf);    
}


void PutInode(int inodeno, Inode* pInode)
{   
    int i;
    int blkno = inodeno / NUM_OF_INODE_PER_BLOCK + INODELIST_BLOCK_FIRST;
    Inode* buf = (Inode*)malloc(BLOCK_SIZE);

    DevReadBlock(blkno, (char*)buf);

    // Copy pInode to buffer
    buf[inodeno].allocBlocks = pInode->allocBlocks;
    buf[inodeno].size = pInode->size;
    buf[inodeno].type = pInode->type;
    for(i = 0; i < NUM_OF_DIRECT_BLOCK_PTR; i++){
        buf[inodeno].dirBlockPtr[i] = pInode->dirBlockPtr[i];
    }

    DevWriteBlock(blkno, (char*)buf);
}

void GetInode(int inodeno, Inode* pInode)
{    //0~15: 3, 16~31: 4, 32~47: 5, 48~63: 6
    int i;
    int blkno = inodeno / NUM_OF_INODE_PER_BLOCK + INODELIST_BLOCK_FIRST;
    Inode* buf = (Inode*)malloc(BLOCK_SIZE);

    DevReadBlock(blkno, (char*)buf);

    // Copy buffer to pInode
    pInode->allocBlocks = buf[inodeno].allocBlocks;
    pInode->size = buf[inodeno].size;
    pInode->type = buf[inodeno].type;
    for(i = 0; i < NUM_OF_DIRECT_BLOCK_PTR; i++){
        pInode->dirBlockPtr[i] = buf[inodeno].dirBlockPtr[i];
    }
}


int GetFreeInodeNum(void)
{
    int i;
    char* buf = (char*)malloc(BLOCK_SIZE);

    DevReadBlock(INODE_BYTEMAP_BLOCK_NUM, buf);

    for(i = 0; i < NUM_OF_INODE_PER_BLOCK * INODELIST_BLOCKS; i++){     //0~63
        if(buf[i] == 0)
            return i;
    }

    return -1;
}


int GetFreeBlockNum(void)
{
    int i;
    char* buf = (char*)malloc(BLOCK_SIZE);

    DevReadBlock(BLOCK_BYTEMAP_BLOCK_NUM, buf);

    for(i = 7; i < BLOCK_SIZE; i++){    //data region start block=7
        if(buf[i] == 0)
            return i;
    }
    
    return -1;
}