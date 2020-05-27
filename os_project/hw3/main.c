#include "fs.h"
#include "disk.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	int blknum, inodenum;
	Inode inode, *pInode;
	int *ptr;
	FileSysInit();
	DevCreateDisk();

	inodenum = GetFreeInodeNum();
	printf("1. %d\n", inodenum);

	SetInodeBytemap(inodenum);
	printf("inode byte map set %d\n", inodenum);

	blknum = GetFreeBlockNum();
	printf("3. %d\n", blknum);

	SetBlockBytemap(blknum);
	blknum = GetFreeBlockNum();
	printf("4. %d\n", blknum);

	inode.allocBlocks = 1;
	inode.size = 2;
	inode.type = 3;
	inode.dirBlockPtr[0] = 0;
	inode.dirBlockPtr[1] = 1;
	inode.dirBlockPtr[2] = 1;
	inode.dirBlockPtr[3] = 1;
	inode.dirBlockPtr[4] = 1;

	PutInode(inodenum, &inode);
	pInode = (Inode*)malloc(sizeof(Inode));
	printf("%d %d %d \n", pInode->allocBlocks, pInode->size, pInode->type);
	GetInode(inodenum, pInode);

	printf("%d %d %d \n", pInode->allocBlocks, pInode->size, pInode->type);
	return 0;
}
 