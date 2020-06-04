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
	MakeDir("/temp/aaa/dir1");
	CreateFile("/temp/aaa/ccc");
	CreateFile("/temp/aaa/ddd");
	CloseFile(0);
	CloseFile(1);
	CloseFile(2);

	printf("============================\n");
	num = EnumerateDirStatus("/temp/aaa", pDirEntry, 20);
	printf("/temp/aaa\n");
	for(i = 0; i < num; i++){
		printf("[%d] name: %s type: %d  inode: %d\n", i,pDirEntry[i].name, pDirEntry[i].type, pDirEntry[i].inodeNum);
	}
	printf("========remove ccc==========\n");
	RemoveFile("/temp/aaa/ccc");
	num = EnumerateDirStatus("/temp/aaa", pDirEntry, 20);
	printf("/temp/aaa\n");
	for(i = 0; i < num; i++){
		printf("[%d] name: %s type: %d  inode: %d\n", i,pDirEntry[i].name, pDirEntry[i].type, pDirEntry[i].inodeNum);
	}
	printf("============================\n");

	CloseFileSystem();
	return 0;
}
 