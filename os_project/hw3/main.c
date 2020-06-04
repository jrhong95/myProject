#include "fs.h"
#include "disk.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

int main(){
	DirEntryInfo pDirEntry[20];
	int i, num;
	CreateFileSystem();

	MakeDir("/temp");
	MakeDir("/temp/aaa");
	MakeDir("/tttt");
	CreateFile("/temp/abc");
	CloseFile(0);
	
	num = EnumerateDirStatus("/temp", pDirEntry, 20);
	for(i = 0; i < num; i++){
		printf("[%d] name: %s type: %d  inode: %d\n", i,pDirEntry[i].name, pDirEntry[i].type, pDirEntry[i].inodeNum);
	}

	printf("============================\n");
	RemoveDir("/temp/aaa");

	num = EnumerateDirStatus("/", pDirEntry, 20);
	for(i = 0; i < num; i++){
		printf("[%d] name: %s type: %d  inode: %d\n", i,pDirEntry[i].name, pDirEntry[i].type, pDirEntry[i].inodeNum);
	}
	
	RemoveFile("/temp/abc");
	CloseFileSystem();
	return 0;
}
 