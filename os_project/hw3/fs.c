#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disk.h"
#include "fs.h"

FileSysInfo* pFileSysInfo;
int level = 0;  //inodex 탐색을 위한 전역변수

int OpenDir(const char* szDirName){
    int inodeno, retval;
    int i, j;
    char delRootName[strlen(szDirName)-1];
    char dirName[MAX_NAME_LEN], nextdirName[strlen(szDirName)-1];
    Inode* pInode = NULL;
    DirEntry* dirEntry = NULL;

    memset(dirName, 0, strlen(dirName));
    memset(nextdirName, 0 , strlen(nextdirName));
    memset(delRootName, 0, strlen(delRootName));

    if(szDirName[0] != '/' && level == 0){
        perror("arg 1 is not directory");
        level = 0;
        return -1;
    }

    // Delete '/'
    for(i = 0; i< strlen(szDirName) - 1; i++){
	    delRootName[i] = szDirName[i+1];
	}
    delRootName[strlen(szDirName)-1] = '\0';
    

    // 현재 디렉토리와 다음 디렉토리 분리
    i = 0;
    while(delRootName[i] != '/' && delRootName[i] != '\0'){
        dirName[i] = delRootName[i];
        i++;
    }
    dirName[i] = '\0';
    j = 0;
    nextdirName[j] = delRootName[i];
    i++;
    j = 1;
    while(delRootName[i] != '\0'){
        nextdirName[j] = delRootName[i];
        i++;
        j++;
    }
    nextdirName[j] = '\0';

    //printf("%s  %s\n", dirName, nextdirName);

    //root dir add szDirName
    pInode = (Inode*)malloc(sizeof(Inode));
    GetInode(level, pInode);
    dirEntry = (DirEntry*)malloc(BLOCK_SIZE);
    if(pInode->type == FILE_TYPE_FILE){
        perror("Can not create file, not Directory\n");
        level = 0;
        return -1;
    }
    DevReadBlock(pInode->dirBlockPtr[0], (char*)dirEntry);

    i = 0;
    while(strcmp(dirEntry[i].name, "EOD") != 0){
        if(strcmp(dirEntry[i].name, dirName) == 0){  //디렉토리가 존재할경우
            level = dirEntry[i].inodeNum;
            
            if(nextdirName[0] == '\0'){             //마지막 디렉토리인 경우
                GetInode(level, pInode);
                level = 0;
                //printf("OpenDir : %d\n", pInode->dirBlockPtr[0]);
                return pInode->dirBlockPtr[0];
            }

            return OpenDir(nextdirName);
        }
        i++;
    }
    // tokenize 이후 값과 이전의 값이 다를 경우-> 하위디렉토리가 존재하는 것
    //하위 디렉토리가 있는데 탐색하려던 디렉토리가 없는 경우
    if(nextdirName[0] != '\0'){
        perror("Not Exist Dir\n");
        level = 0;
        return -1;
    }
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int		CreateFile(const char* szFileName)
{
    int inodeno = GetFreeInodeNum();    //allocating inode
    int i, j;
    int dirFreeIndex;                   //find dirEntry Free index
    char delRootName[strlen(szFileName)-1];
    char name[MAX_NAME_LEN], nextName[strlen(szFileName)-1];
    Inode* pInode = NULL;
    DirEntry* dirEntry = NULL;
    File *file = NULL;
    FileSysInfo* filesys = NULL;
    memset(name, 0, strlen(name));
    memset(nextName, 0 , strlen(nextName));
    memset(delRootName, 0, strlen(delRootName));

    // arg is not dir when root dir
    if(szFileName[0] != '/' && level == 0){
        perror("MakeDir(arg) arg is not directory");
        level = 0;
        return -1;
    }

    // Delete '/'
    for(i = 0; i< strlen(szFileName) - 1; i++){
	    delRootName[i] = szFileName[i+1];
	}
    delRootName[strlen(szFileName)-1] = '\0';
    

    // 현재 디렉토리와 다음 디렉토리 분리
    i = 0;
    while(delRootName[i] != '/' && delRootName[i] != '\0'){
        name[i] = delRootName[i];
        i++;
    }
    name[i] = '\0';
    j = 0;
    nextName[j] = delRootName[i];
    i++;
    j = 1;
    while(delRootName[i] != '\0'){
        nextName[j] = delRootName[i];
        i++;
        j++;
    }
    nextName[j] = '\0';

    //printf("%s  %s\n", name, nextName);

    //find Inode and block
    pInode = (Inode*)malloc(sizeof(Inode));
    GetInode(level, pInode);
    dirEntry = (DirEntry*)malloc(BLOCK_SIZE);
    
    if(pInode->type == FILE_TYPE_FILE){
        perror("Can not create file, not Directory");
        level = 0;
        return -1;
    }
    DevReadBlock(pInode->dirBlockPtr[0], (char*)dirEntry);

    i = 0;
    while(strcmp(dirEntry[i].name, "EOD") != 0){
        if(strcmp(dirEntry[i].name, name) == 0){  //디렉토리가 존재할경우
            if(nextName[0] == '\0'){             //다음 디렉토리가 없을 경우 
                perror("Already Exist File");
                level = 0;
                return -1;
            }
            level = dirEntry[i].inodeNum;
            return CreateFile(nextName);
        }
        i++;
    }
    // tokenize 이후 값과 이전의 값이 다를 경우-> 하위디렉토리가 존재하는 것
    //하위 디렉토리가 있는데 탐색하려던 디렉토리가 없는 경우
    if(nextName[0] != '\0'){
        perror("Not Exist Dir");
        level = 0;
        return -1;
    }
    dirFreeIndex = 0;
    while(strcmp(dirEntry[dirFreeIndex].name, "EOD")){
        dirFreeIndex++;
    }
    dirEntry[dirFreeIndex].inodeNum = inodeno;
    strcpy(dirEntry[dirFreeIndex].name, name);
    dirEntry[dirFreeIndex + 1].inodeNum = -1;
    strcpy(dirEntry[dirFreeIndex + 1].name, "EOD");
    DevWriteBlock(pInode->dirBlockPtr[0], (char*)dirEntry);

    // Set Inode 
    pInode = (Inode*)malloc(sizeof(Inode));
    GetInode(inodeno, pInode);
    pInode->allocBlocks = 0;     /////// empty File
    pInode->size = 0;
    pInode->type = FILE_TYPE_FILE;
    pInode->dirBlockPtr[0] = -1; ////// block not allocated
    PutInode(inodeno, pInode);

    SetInodeBytemap(inodeno);

    //change fileSysInfo
    filesys = (FileSysInfo*)malloc(BLOCK_SIZE);
    DevReadBlock(FILESYS_INFO_BLOCK, (char*)filesys);
    filesys->numAllocInodes++;
    DevWriteBlock(FILESYS_INFO_BLOCK, (char*)filesys);
    pFileSysInfo = filesys;
    level = 0;
    printf("file %s creates succuss\n", name);////////////////////////////////

    i = 0;
    while(pFileDesc[i].bUsed != 0){
        i++;
    }
    file = (File*)malloc(sizeof(File));
    file->fileOffset = i;
    file->inodeNum = inodeno;

    pFileDesc[i].bUsed = 1;
    pFileDesc[i].pOpenFile = file;

    return i;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int		OpenFile(const char* szFileName)
{
    int inodeno = GetFreeInodeNum();    //allocating inode
    int i, j;
    int fileInodeNum;                   
    char delRootName[strlen(szFileName)-1];
    char name[MAX_NAME_LEN], nextName[strlen(szFileName)-1];
    Inode* pInode = NULL;
    DirEntry* dirEntry = NULL;
    File *file = NULL;
    memset(name, 0, strlen(name));
    memset(nextName, 0 , strlen(nextName));
    memset(delRootName, 0, strlen(delRootName));

    // arg is not dir when root dir
    if(szFileName[0] != '/' && level == 0){
        perror("MakeDir(arg) arg is not directory");
        return -1;
    }

    // Delete '/'
    for(i = 0; i< strlen(szFileName) - 1; i++){
	    delRootName[i] = szFileName[i+1];
	}
    delRootName[strlen(szFileName)-1] = '\0';
    

    // 현재 디렉토리와 다음 디렉토리 분리
    i = 0;
    while(delRootName[i] != '/' && delRootName[i] != '\0'){
        name[i] = delRootName[i];
        i++;
    }
    name[i] = '\0';
    if(strcmp(delRootName, name) != 0){
        j = 0;
        nextName[j] = delRootName[i];
        i++;
        j++;
        while(delRootName[i] != '\0'){
            nextName[j] = delRootName[i];
            i++;
            j++;
        }
        nextName[j] = '\0';
    }

    //find Inode and block
    pInode = (Inode*)malloc(sizeof(Inode));
    GetInode(level, pInode);
    dirEntry = (DirEntry*)malloc(BLOCK_SIZE);
    //printf("level: %d\npInode->type: %d\n", level, pInode->type);
    // 파일이 중간에 경로로 삽입된 경우
    if(pInode->type == FILE_TYPE_FILE && nextName[0] != '\0'){
        printf("Can not open file, %s is not Directory\n", name);
        level = 0;
        return -1;
    }
    DevReadBlock(pInode->dirBlockPtr[0], (char*)dirEntry);

    i = 0;
    while(strcmp(dirEntry[i].name, "EOD") != 0){
        if(strcmp(dirEntry[i].name, name) == 0){  //디렉토리가 존재할경우
            if(nextName[0] == '\0'){              //파일에 도달한 경우
                //파일인지에 대한 검증
                break;
            }
            level = dirEntry[i].inodeNum;
            return OpenFile(nextName);              //하위 디렉토리 탐색
        }
        i++;
    }

    //하위 디렉토리가 있는데 탐색하려던 디렉토리가 없는 경우
    if(nextName[0] != '\0'){
        perror("Not Exist Dir\n");
        level = 0;
        return -1;
    }
    
    fileInodeNum = dirEntry[i].inodeNum;

    i = 0;
    while(pFileDesc[i].bUsed != 0){
        if(pFileDesc[i].bUsed == 1 && pFileDesc[i].pOpenFile->inodeNum == fileInodeNum){
            // Already Open
            printf("%s is already opened\n", name);
            level = 0;
            return -1;
        }
        i++;
    }

    file = (File*)malloc(sizeof(File));
    file->fileOffset = 0;
    file->inodeNum = fileInodeNum;

    pFileDesc[i].bUsed = 1;
    pFileDesc[i].pOpenFile = file;

    level = 0;
    return i;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int		WriteFile(int fileDesc, char* pBuffer, int length)
{
    int blkno = GetFreeBlockNum();
    int fdInode, offset, freeLogicalBlockNum;
    Inode *pInode = NULL;
    FileSysInfo *filesys = NULL;
    char *buf = NULL;

    if(pFileDesc[fileDesc].bUsed == 0){
        printf("File is closed\n");
        return -1;
    }
    //fd infomation save
    fdInode = pFileDesc[fileDesc].pOpenFile->inodeNum;
    offset = pFileDesc[fileDesc].pOpenFile->fileOffset;

    //Update fd's inode
    pInode = (Inode*)malloc(sizeof(Inode));
    GetInode(fdInode, pInode);
    freeLogicalBlockNum = offset / length;
    // file offset is bigger than Direct Block ptr num
    if(freeLogicalBlockNum >= NUM_OF_DIRECT_BLOCK_PTR){ 
        printf("file is full\n");
        return -1;
    }
    pInode->dirBlockPtr[freeLogicalBlockNum] = blkno;
    pInode->allocBlocks++;
    pInode->size += length;
    PutInode(fdInode, pInode);

    //Update FileSysInfo Block
    filesys = (FileSysInfo*)malloc(BLOCK_SIZE);
    filesys->numAllocBlocks++;
    filesys->numFreeBlocks--;
    DevWriteBlock(FILESYS_INFO_BLOCK, (char*)filesys);
    pFileSysInfo = filesys;

    SetBlockBytemap(blkno);
    //FileOffset += BLOCK_SIZE
    pFileDesc[fileDesc].pOpenFile->fileOffset += length;

    //Write buffer to block
    buf = (char*)malloc(BLOCK_SIZE);
    buf = pBuffer;
    DevWriteBlock(blkno, buf);

    return length;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int		ReadFile(int fileDesc, char* pBuffer, int length)
{
    int blkno ;
    int fdInode, offset, curLogicalBlockNum;
    Inode *pInode = NULL;
    char *buf = NULL;

    if(pFileDesc[fileDesc].bUsed == 0){
        printf("File is closed\n");
        return -1;
    }

    fdInode = pFileDesc[fileDesc].pOpenFile->inodeNum;
    offset = pFileDesc[fileDesc].pOpenFile->fileOffset;

    //Get Inode Info
    pInode = (Inode*)malloc(sizeof(Inode));
    GetInode(fdInode, pInode);
    curLogicalBlockNum = offset / length;

    if(curLogicalBlockNum >= NUM_OF_DIRECT_BLOCK_PTR){
        printf("Read Fail\n");
        return -1;
    }
    blkno = pInode->dirBlockPtr[curLogicalBlockNum];

    buf = (char*)malloc(BLOCK_SIZE);
    DevReadBlock(blkno, buf);

    pBuffer = buf;
    pFileDesc[fileDesc].pOpenFile->fileOffset += length;

    return length;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int		CloseFile(int fileDesc)
{
    if(pFileDesc[fileDesc].bUsed == 0){
        printf("File is already closed\n");
        return -1;
    }
    pFileDesc[fileDesc].bUsed = 0;
    free(pFileDesc[fileDesc].pOpenFile);
    pFileDesc[fileDesc].pOpenFile = NULL;

    return fileDesc;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int		RemoveFile(const char* szFileName)
{

}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int		MakeDir(const char* szDirName)
{
    int blkno = GetFreeBlockNum();      //allocating block
    int inodeno = GetFreeInodeNum();    //allocating inode
    int i, j, dirFreeIndex;
    char delRootName[strlen(szDirName)-1];
    char dirName[MAX_NAME_LEN], nextdirName[strlen(szDirName)-1];
    Inode* pInode = NULL;
    DirEntry* dirEntry = NULL;
    FileSysInfo* filesys = NULL;

    // arg is not dir when root dir
    if(szDirName[0] != '/' && level == 0){
        perror("MakeDir(arg) arg is not directory");
        level = 0;
        return -1;
    }

    // Delete '/'
    for(i = 0; i< strlen(szDirName) - 1; i++){
	    delRootName[i] = szDirName[i+1];
	}
    delRootName[strlen(szDirName)-1] = '\0';
    

    // 현재 디렉토리와 다음 디렉토리 분리
    i = 0;
    while(delRootName[i] != '/' && delRootName[i] != '\0'){
        dirName[i] = delRootName[i];
        i++;
    }
    dirName[i] = '\0';
    j = 0;
    nextdirName[j] = delRootName[i];
    i++;
    j = 1;
    while(delRootName[i] != '\0'){
        nextdirName[j] = delRootName[i];
        i++;
        j++;
    }
    nextdirName[j] = '\0';

    //printf("%s  %s\n", dirName, nextdirName);

    //root dir add szDirName
    pInode = (Inode*)malloc(sizeof(Inode));
    GetInode(level, pInode);
    dirEntry = (DirEntry*)malloc(BLOCK_SIZE);
    if(pInode->type == FILE_TYPE_FILE){
        perror("Can not create file, not Directory\n");
        level = 0;
        return -1;
    }
    DevReadBlock(pInode->dirBlockPtr[0], (char*)dirEntry);

    i = 0;
    while(strcmp(dirEntry[i].name, "EOD") != 0){
        if(strcmp(dirEntry[i].name, dirName) == 0){  //디렉토리가 존재할경우
            if(nextdirName[0] == '\0'){             //다음 디렉토리가 없을 경우 
                perror("Already Exist Dir\n");
                level = 0;
                return -1;
            }
            level = dirEntry[i].inodeNum;
            return MakeDir(nextdirName);
        }
        i++;
    }
    // tokenize 이후 값과 이전의 값이 다를 경우-> 하위디렉토리가 존재하는 것

    //하위 디렉토리가 있는데 탐색하려던 디렉토리가 없는 경우
    if(nextdirName[0] != '\0'){
        perror("Not Exist Dir\n");
        level = 0;
        return -1;
    }
    dirFreeIndex = 0;
    while(strcmp(dirEntry[dirFreeIndex].name, "EOD")){//End of Directory
        dirFreeIndex++;
    }
    dirEntry[dirFreeIndex].inodeNum = inodeno;
    strcpy(dirEntry[dirFreeIndex].name, dirName);

    //set End Dirname
    dirEntry[dirFreeIndex + 1].inodeNum = -1;
    strcpy(dirEntry[dirFreeIndex + 1].name, "EOD");
    DevWriteBlock(pInode->dirBlockPtr[0], (char*)dirEntry);

    //szDirName dir make
    dirEntry = (DirEntry*)malloc(BLOCK_SIZE);
    dirEntry[0].inodeNum = inodeno;
    strcpy(dirEntry[0].name, ".");
    dirEntry[1].inodeNum = level;
    strcpy(dirEntry[1].name, "..");
    dirEntry[2].inodeNum = -1;
    strcpy(dirEntry[2].name, "EOD");
    DevWriteBlock(blkno, (char*)dirEntry);

    // Set Inode 
    pInode = (Inode*)malloc(sizeof(Inode));
    GetInode(inodeno, pInode);
    pInode->allocBlocks = 1;
    pInode->size = 512;
    pInode->type = FILE_TYPE_DIR;
    pInode->dirBlockPtr[0] = blkno;
    PutInode(inodeno, pInode);

    SetBlockBytemap(blkno);
    SetInodeBytemap(inodeno);

    //change fileSysInfo
    filesys = (FileSysInfo*)malloc(BLOCK_SIZE);
    DevReadBlock(FILESYS_INFO_BLOCK, (char*)filesys);
    filesys->numAllocBlocks++;
    filesys->numFreeBlocks--;
    filesys->numAllocInodes++;
    DevWriteBlock(FILESYS_INFO_BLOCK, (char*)filesys);
    pFileSysInfo = filesys;
    level = 0;
    printf("Directory %s creates succuss\n", dirName);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int		RemoveDir(const char* szDirName)
{
    Inode *pInode = NULL;
    DirEntry *dirEntry = (DirEntry*)malloc(BLOCK_SIZE);
    int i, curInodeNo, prevInodeNo, curBlkNum = OpenDir(szDirName);

    DevReadBlock(curBlkNum, (char*)dirEntry);
    // szDir is not Empty
    if(strcmp(dirEntry[2].name, "EOD") != 0){
        printf("%s is not Empty\n", szDirName);
        return -1;
    }
    // go to ..
    prevInodeNo = dirEntry[1].inodeNum;
    curInodeNo = dirEntry[0].inodeNum;
    pInode = (Inode*)malloc(sizeof(Inode));
    GetInode(prevInodeNo, pInode);

    curBlkNum = pInode->dirBlockPtr[0];
    //reset block
    dirEntry = (DirEntry*)calloc(BLOCK_SIZE, 1);
    for(i = 0; i < NUM_OF_DIRENT_PER_BLOCK; i++)
        printf("%s %d\n", dirEntry[i].name, dirEntry[i].inodeNum);

    i = 0;
    while(dirEntry[i].inodeNum != curInodeNo){
        i++;
    }
    return 0;
}

int   EnumerateDirStatus(const char* szDirName, DirEntryInfo* pDirEntry, int dirEntrys)
{
    int curBlkNum = OpenDir(szDirName);
    int i, retval = 0;
    Inode *pInode = NULL;
    DirEntry *dirEntry = (DirEntry*)malloc(BLOCK_SIZE); 
    DevReadBlock(curBlkNum, (char*)dirEntry);

    for(i = 0; i < dirEntrys; i++){
        if(dirEntry[i].inodeNum == -1)
            break;

        strcpy(pDirEntry[i].name, dirEntry[i].name);
        pDirEntry[i].inodeNum = dirEntry[i].inodeNum;

        pInode = (Inode*)malloc(sizeof(Inode));
        GetInode(pDirEntry[i].inodeNum, pInode);
        pDirEntry[i].type = pInode->type;

        if((strcmp(dirEntry[i].name, ".") == 0) || 
            (strcmp(dirEntry[i].name, "..") == 0)){
                pDirEntry[i].type = FILE_TYPE_DEV;      //??????
            }
        
        retval++;
    }

    return retval;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////f
void	CreateFileSystem()
{
    int blkno, inodeno;
    DirEntry* dirEntry;
    Inode* pInode;

    DevCreateDisk();
    FileSysInit();
    blkno = GetFreeBlockNum();
    inodeno = GetFreeInodeNum();

    dirEntry = (DirEntry*)malloc(BLOCK_SIZE);
    dirEntry[0].inodeNum = 0;
    strcpy(dirEntry[0].name, ".");
    dirEntry[1].inodeNum = -1;
    strcpy(dirEntry[1].name, "EOD");
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
    DevWriteBlock(FILESYS_INFO_BLOCK, (char*)pFileSysInfo);

    // Block, Inode set root
    SetBlockBytemap(blkno);
    SetInodeBytemap(inodeno);
    
    // Change Inode
    pInode = (Inode*)malloc(sizeof(Inode));
    GetInode(inodeno, pInode);
    pInode->dirBlockPtr[0] = 7; //root dir block
    pInode->allocBlocks = 1;
    pInode->size = 512;
    pInode->type = FILE_TYPE_DIR;
    PutInode(inodeno, pInode);
}


void	OpenFileSystem()
{
    DevOpenDisk();
}


void	CloseFileSystem()
{
    DevCloseDisk();
}

int		GetFileStatus(const char* szPathName, FileStatus* pStatus)
{
    
}
