#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disk.h"
#include "fs.h"

FileSysInfo* pFileSysInfo;
int		CreateFile(const char* szFileName)
{

}

int		OpenFile(const char* szFileName)
{

}


int		WriteFile(int fileDesc, char* pBuffer, int length)
{

}

int		ReadFile(int fileDesc, char* pBuffer, int length)
{

}


int		CloseFile(int fileDesc)
{

}

int		RemoveFile(const char* szFileName)
{

}


int		MakeDir(const char* szDirName)
{

}


int		RemoveDir(const char* szDirName)
{

}

int   EnumerateDirStatus(const char* szDirName, DirEntryInfo* pDirEntry, int dirEntrys)
{

}


void	CreateFileSystem()
{
    int blkno, inodeno;
    DirEntry* dirEntry;
    Inode* pInode;

    FileSysInit();
    blkno = GetFreeBlockNum();
    inodeno = GetFreeInodeNum();

    dirEntry = (DirEntry*)malloc(BLOCK_SIZE);
    dirEntry[0].inodeNum = 0;
    strcpy(dirEntry[0].name, ".");
    DevWriteBlock(blkno, (char*)dirEntry);

    //pFileSysInfo Init
    pFileSysInfo = (FileSysInfo*)malloc(BLOCK_SIZE);
    pFileSysInfo->blocks = 7;                        //0~6 block
    pFileSysInfo->rootInodeNum = 0;                  //
    pFileSysInfo->diskCapacity = FS_DISK_CAPACITY;   
    pFileSysInfo->numAllocBlocks = 0;               //
    pFileSysInfo->numFreeBlocks = FS_DISK_CAPACITY/BLOCK_SIZE - 7;
    pFileSysInfo->numAllocInodes = 0;
    pFileSysInfo->blockBytemapBlock = 7;
    pFileSysInfo->inodeBytemapBlock = 0;
    pFileSysInfo->inodeListBlock = INODELIST_BLOCK_FIRST;
    pFileSysInfo->dataRegionBlock = 7;

    //pFileSysInfo change 
    pFileSysInfo->numAllocBlocks++;               
    pFileSysInfo->numFreeBlocks--;
    pFileSysInfo->numAllocInodes++;

    // Block, Inode set root
    SetBlockBytemap(blkno);
    SetInodeBytemap(inodeno);
    
    // Change Inode
    (Inode*)malloc(sizeof(Inode));
    GetInode(inodeno, pInode);
    pInode->dirBlockPtr[0] = 7; //root dir block
    pInode->allocBlocks = 1;
    pInode->size = 512;
    pInode->type = FILE_TYPE_DIR;
    PutInode(inodeno, pInode);
}


void	OpenFileSystem()
{

}


void	CloseFileSystem()
{

}
