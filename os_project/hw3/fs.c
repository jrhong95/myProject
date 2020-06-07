#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "disk.h"
#include "fs.h"

FileSysInfo* pFileSysInfo;
int level = 0;  //inodex 탐색을 위한 전역변수

//
//  입력받은 target의 inode number를 리턴한다
//
int OpenDir(const char* szDirName){
    int inodeno, retval, dirPtrIndex = 0;
    int i, j;
    char delRootName[strlen(szDirName)-1];
    char dirName[MAX_NAME_LEN], nextdirName[strlen(szDirName)-1];
    Inode* pInode = NULL;
    DirEntry* dirEntry = NULL;

    memset(dirName, 0, strlen(dirName));
    memset(nextdirName, 0 , strlen(nextdirName));
    memset(delRootName, 0, strlen(delRootName));

    // Open Root dir
    if(strcmp(szDirName, "/") == 0)
        return 7;
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
    DevReadBlock(pInode->dirBlockPtr[dirPtrIndex], (char*)dirEntry);

    i = 0;
    while(strcmp(dirEntry[i].name, "EOD") != 0){
        if(strcmp(dirEntry[i].name, dirName) == 0){  //디렉토리가 존재할경우
            level = dirEntry[i].inodeNum;
            
            if(nextdirName[0] == '\0'){             //마지막 디렉토리인 경우
                GetInode(level, pInode);
                retval = level;
                level = 0;
                return retval;
            }
            return OpenDir(nextdirName);
        }
        i++;
        if(i == 31 && pInode->dirBlockPtr[dirPtrIndex + 1] != -1){
            DevReadBlock(pInode->dirBlockPtr[++dirPtrIndex], (char*)dirEntry);
            i = 0;
        }
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
        perror("CreateFile(arg) arg is not directory");
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
    for(i = 0; i < NUM_OF_DIRECT_BLOCK_PTR; i++){
        pInode->dirBlockPtr[i] = -1; ////// block not allocated
    }

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
        perror("OpenFile(arg) arg is not directory");
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
    DevReadBlock(FILESYS_INFO_BLOCK, (char*)filesys);
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
    int i, j, fileDirEntryIndex;
    int delFileInodeNum, blkno, prevBlkNum;
    char delRootName[strlen(szFileName)-1];
    char name[MAX_NAME_LEN], nextName[strlen(szFileName)-1];
    char *buf;
    Inode* pInode = NULL;
    DirEntry* dirEntry = NULL;
    File *file = NULL;
    FileSysInfo* filesys = NULL;
    
    memset(name, 0, strlen(name));
    memset(nextName, 0 , strlen(nextName));
    memset(delRootName, 0, strlen(delRootName));

    // arg is not dir when root dir
    if(szFileName[0] != '/' && level == 0){
        perror("RemoveFile Error: arg is not directory");
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

    //find Inode and block
    pInode = (Inode*)malloc(sizeof(Inode));
    GetInode(level, pInode);
    dirEntry = (DirEntry*)malloc(BLOCK_SIZE);
    
    if(pInode->type == FILE_TYPE_FILE){
        perror("RemoveFile Error: Can not create file, not Directory");
        level = 0;
        return -1;
    }
    DevReadBlock(pInode->dirBlockPtr[0], (char*)dirEntry);
    prevBlkNum = pInode->dirBlockPtr[0];

    i = 0;
    while(strcmp(dirEntry[i].name, "EOD") != 0){
        if(strcmp(dirEntry[i].name, name) == 0){  //디렉토리가 존재할경우
            if(nextName[0] != '\0'){             //다음 디렉토리가 있을경우 
                level = dirEntry[i].inodeNum;
                return RemoveFile(nextName);
            }
        }
        i++;
    }
    //하위 디렉토리가 있는데 탐색하려던 디렉토리가 없는 경우
    if(nextName[0] != '\0'){
        perror("RemoveFile Error: Not Exist Dir");
        level = 0;
        return -1;
    }
    i = 0;
    while(strcmp(dirEntry[i].name, name) != 0){
        i++;
    }
    fileDirEntryIndex = i;
    delFileInodeNum = dirEntry[i].inodeNum;
    for(i = 0; i < MAX_FD_ENTRY_MAX; i++){
        if(pFileDesc[i].bUsed == 1
        && pFileDesc[i].pOpenFile->inodeNum == delFileInodeNum){
            perror("RemoveFile Error: File is Opened");
            return -1;
        }
    }

    GetInode(delFileInodeNum, pInode);
    if(pInode->type != FILE_TYPE_FILE){
        perror("RemoveFile Error: Not File");
        return -1;
    }

    filesys = (FileSysInfo*)malloc(BLOCK_SIZE);
    DevReadBlock(FILESYS_INFO_BLOCK, (char*)filesys);
    // Reset allocated blocks
    for(i = 0; i < pInode->size / BLOCK_SIZE; i++){
        blkno = pInode->allocBlocks;
        buf = (char*)calloc(BLOCK_SIZE, sizeof(char));
        DevWriteBlock(blkno, buf);
        ResetBlockBytemap(blkno);
        //Update fileSysInfo
        filesys->numAllocBlocks--;
        filesys->numFreeBlocks++;
        //free
    }
    // Reset allocated Inode
    pInode = (Inode*)calloc(sizeof(Inode) / sizeof(int), sizeof(int));
    PutInode(delFileInodeNum, pInode);
    ResetInodeBytemap(delFileInodeNum);
    filesys->numAllocInodes--;

    while(strcmp(dirEntry[fileDirEntryIndex].name, "EOD") != 0){
        strcpy(dirEntry[fileDirEntryIndex].name, dirEntry[fileDirEntryIndex + 1].name);
        dirEntry[fileDirEntryIndex].inodeNum = dirEntry[fileDirEntryIndex + 1].inodeNum;
        fileDirEntryIndex++;
    }

    //Delete target dir & logical block update
    DevWriteBlock(prevBlkNum, (char*)dirEntry);
    DevWriteBlock(FILESYS_INFO_BLOCK, (char*)filesys);
    pFileSysInfo = filesys;
    level = 0;
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int		MakeDir(const char* szDirName)
{
    int blkno;      //allocating block
    int inodeno;   //allocating inode
    int i, j, dirFreeIndex, directPtrIndex = 0;
    char delRootName[strlen(szDirName)-1];
    char dirName[MAX_NAME_LEN], nextdirName[strlen(szDirName)-1];
    Inode* pInode = NULL;
    DirEntry* dirEntry = NULL;
    FileSysInfo* filesys = NULL;

    memset(dirName, 0, MAX_NAME_LEN);
    memset(nextdirName, 0 , strlen(szDirName)-1);
    memset(delRootName, 0, strlen(szDirName)-1);

    // arg is not dir when root dir
    if(szDirName[0] != '/' && level == 0){
        perror("MakeDir(arg) arg is not directory");
        level = 0;
        return -1;
    }

    // Delete '/'MakeD
    for(i = 0; i< strlen(szDirName) - 1; i++){
	    delRootName[i] = szDirName[i+1];
	}
    delRootName[strlen(szDirName)-1] = '\0';
    

    // 현재 디렉토리와 다음 디렉토리 분리MakeD
    i = 0;
    while(delRootName[i] != '/' && delRootName[i] != '\0'){
        dirName[i] = delRootName[i];
        i++;
    }
    dirName[i] = '\0';
    if(strlen(dirName) != strlen(delRootName)){
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
    }
    //\printf("%s  %s\n", dirName, nextdirName);

    //root dir add szDirName
    pInode = (Inode*)malloc(sizeof(Inode));
    GetInode(level, pInode);

    if(pInode->type == FILE_TYPE_FILE){
        perror("Can not create file, not Directory\n");
        level = 0;
        return -1;
    }

    dirEntry = (DirEntry*)malloc(BLOCK_SIZE);
    DevReadBlock(pInode->dirBlockPtr[directPtrIndex], (char*)dirEntry);

    i = -1;
    while(strcmp(dirEntry[i].name, "EOD") != 0){
        i++;
        if(i == 31 && pInode->dirBlockPtr[directPtrIndex+1] == -1){ //next dptr is null
            break;
        }
        else if(i == 31){
            directPtrIndex++;
            DevReadBlock(pInode->dirBlockPtr[directPtrIndex], (char*)dirEntry);
            i = 0;
            continue;
        }

        if(strcmp(dirEntry[i].name, dirName) == 0){  //디렉토리가 존재할경우
            if(nextdirName[0] == '\0'){             //다음 디렉토리가 없을 경우 
                perror("Already Exist Dir\n");
                level = 0;
                return -1;
            }
            level = dirEntry[i].inodeNum;
            return MakeDir(nextdirName);
        }
    }
    // tokenize 이후 값과 이전의 값이 다를 경우-> 하위디렉토리가 존재하는 것

    //하위 디렉토리가 있는데 탐색하려던 디렉토리가 없는 경우
    if(nextdirName[0] != '\0'){
        perror("Not Exist Dir\n");
        level = 0;
        return -1;
    }
    dirFreeIndex = 0;
    while(strcmp(dirEntry[dirFreeIndex].name, "EOD") != 0){//End of Directory
        dirFreeIndex++;
    }

    filesys = (FileSysInfo*)malloc(BLOCK_SIZE);
    DevReadBlock(FILESYS_INFO_BLOCK, (char*)filesys);
    blkno = GetFreeBlockNum();
    if(dirFreeIndex == 31){                         //Last Entry
        directPtrIndex++;

        pInode->dirBlockPtr[directPtrIndex] = blkno;
        printf("new block: %d\n", blkno);
        SetBlockBytemap(blkno);
        filesys->numAllocBlocks++;
        filesys->numFreeBlocks--;
        PutInode(level, pInode);
        dirEntry = (DirEntry*)malloc(BLOCK_SIZE);
        dirFreeIndex = 0;
        blkno = GetFreeBlockNum();
    }
    inodeno = GetFreeInodeNum();
    dirEntry[dirFreeIndex].inodeNum = inodeno;
    strcpy(dirEntry[dirFreeIndex].name, dirName);

    //set End Dirname
    dirEntry[dirFreeIndex + 1].inodeNum = -1;
    strcpy(dirEntry[dirFreeIndex + 1].name, "EOD");
    DevWriteBlock(pInode->dirBlockPtr[directPtrIndex], (char*)dirEntry);

    //szDirName dir make
    dirEntry = (DirEntry*)malloc(BLOCK_SIZE);
    dirEntry[0].inodeNum = inodeno;
    strcpy(dirEntry[0].name, ".");
    dirEntry[1].inodeNum = level;
    strcpy(dirEntry[1].name, "..");
    dirEntry[2].inodeNum = -1;
    strcpy(dirEntry[2].name, "EOD");
    blkno = GetFreeBlockNum();
    DevWriteBlock(blkno, (char*)dirEntry);


    // Set Inode 
    pInode = (Inode*)malloc(sizeof(Inode));
    GetInode(inodeno, pInode);
    pInode->allocBlocks = 1;
    pInode->size = 512;
    pInode->type = FILE_TYPE_DIR;
    pInode->dirBlockPtr[0] = blkno;
    pInode->dirBlockPtr[1] = -1;
    pInode->dirBlockPtr[2] = -1;
    pInode->dirBlockPtr[3] = -1;
    pInode->dirBlockPtr[4] = -1;
    PutInode(inodeno, pInode);

    SetBlockBytemap(blkno);
    SetInodeBytemap(inodeno);

    //change fileSysInfo
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
    FileSysInfo *filesys = NULL;
    int i, dirEntryIndex, curBlkNum, prevBlkNum;
    int prevInodeNo, curInodeNo = OpenDir(szDirName); // Open target dir

    
    pInode = (Inode*)malloc(sizeof(Inode));
    GetInode(curInodeNo, pInode);
    curBlkNum = pInode->dirBlockPtr[0];

    DevReadBlock(curBlkNum, (char*)dirEntry);
    // szDir is not Empty
    if(strcmp(dirEntry[2].name, "EOD") != 0){
        printf("%s is not Empty\n", szDirName);
        return -1;
    }

    prevInodeNo = dirEntry[1].inodeNum;
    curInodeNo = dirEntry[0].inodeNum;

    // Delete target dir inode 
    pInode = (Inode*)calloc(sizeof(Inode)/sizeof(int), sizeof(int));
    PutInode(curInodeNo, pInode);
    ResetInodeBytemap(curInodeNo);

    // Delete target dir block
    dirEntry = (DirEntry*)calloc(NUM_OF_DIRENT_PER_BLOCK, sizeof(DirEntry));
    DevWriteBlock(curBlkNum, (char*)dirEntry);
    ResetBlockBytemap(curBlkNum);
/*     for(i = 0; i < NUM_OF_DIRENT_PER_BLOCK; i++)
        printf("%s %d\n", dirEntry[i].name, dirEntry[i].inodeNum); */

    //Prev Directory Inode & block Read
    pInode = (Inode*)malloc(sizeof(Inode));
    GetInode(prevInodeNo, pInode);
    prevBlkNum = pInode->dirBlockPtr[0];
    dirEntry = (DirEntry*)malloc(BLOCK_SIZE);
    DevReadBlock(prevBlkNum, (char*)dirEntry);

    //Find target dir
    dirEntryIndex = 0;
    while(dirEntry[dirEntryIndex].inodeNum != curInodeNo){
        dirEntryIndex++;
    }
    //Delete target dir & logical block update
    while(strcmp(dirEntry[dirEntryIndex].name, "EOD") != 0){
        strcpy(dirEntry[dirEntryIndex].name, dirEntry[dirEntryIndex + 1].name); //next dir copy
        dirEntry[dirEntryIndex].inodeNum = dirEntry[dirEntryIndex + 1].inodeNum;
        dirEntryIndex++;
    }
    DevWriteBlock(prevBlkNum, (char*)dirEntry);
    // File System Update
    filesys = (FileSysInfo*)malloc(BLOCK_SIZE);
    DevReadBlock(FILESYS_INFO_BLOCK, (char*)filesys);
    filesys->numAllocBlocks--;
    filesys->numAllocInodes--;
    filesys->numFreeBlocks++;
    DevWriteBlock(FILESYS_INFO_BLOCK, (char*)filesys);
    pFileSysInfo = filesys;
    return 0;
}

int   EnumerateDirStatus(const char* szDirName, DirEntryInfo* pDirEntry, int dirEntrys)
{
    int curInodeNo = OpenDir(szDirName);
    int curBlkNum, directPtrIndex = 0;
    int i, dentryCount = 0, retval = 0;
    Inode *pInode = NULL, *newpInode = NULL;
    DirEntry *dirEntry = (DirEntry*)malloc(BLOCK_SIZE); 

    pInode = (Inode*)malloc(sizeof(Inode));
    GetInode(curInodeNo, pInode);
    curBlkNum = pInode->dirBlockPtr[directPtrIndex];
    DevReadBlock(curBlkNum, (char*)dirEntry);

    for(i = 0; i <= dirEntrys; i++){
        if(dirEntry[i].inodeNum == -1 && pInode->dirBlockPtr[directPtrIndex + 1] != -1){
            curBlkNum = pInode->dirBlockPtr[++directPtrIndex];
            DevReadBlock(curBlkNum, (char*)dirEntry);
            i = -1;
            continue;
        }
        else if(dirEntry[i].inodeNum == -1)
            break;

        strcpy(pDirEntry[dentryCount].name, dirEntry[i].name);
        pDirEntry[dentryCount].inodeNum = dirEntry[i].inodeNum;

        newpInode = (Inode*)malloc(sizeof(Inode));
        GetInode(pDirEntry[dentryCount].inodeNum, newpInode);
        pDirEntry[dentryCount].type = newpInode->type;

        dentryCount++;
    }

    return dentryCount;
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
    pInode->dirBlockPtr[1] = -1;
    pInode->dirBlockPtr[2] = -1;
    pInode->dirBlockPtr[3] = -1;
    pInode->dirBlockPtr[4] = -1;
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
    int i, inodeno = OpenDir(szPathName);
    Inode *pInode = NULL;

    if(inodeno == -1){
        perror("GetFileStatus Error: Open Path error.");
        return -1;
    }

    pInode = (Inode*)malloc(sizeof(Inode));
    GetInode(inodeno, pInode);

    pStatus->allocBlocks = pInode->allocBlocks;
    pStatus->size = pInode->size;
    pStatus->type = pInode->type;
    for(i = 0; i < NUM_OF_DIRECT_BLOCK_PTR; i++){
        pStatus->dirBlockPtr[i] = pInode->dirBlockPtr[i];
    }
    
    return 0;
}
