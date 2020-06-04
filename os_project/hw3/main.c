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
	int i;
	CreateFileSystem();

	MakeDir("/temp");
	MakeDir("/temp/aaa");
	MakeDir("/tttt");
	CreateFile("/temp/abc");

	CloseFile(0);
	OpenFile("/");

	for(i = 0; i < EnumerateDirStatus("/temp", pDirEntry, 20); i++){
		printf("[%d] name: %s type: %d  inode: %d\n", i,pDirEntry[i].name, pDirEntry[i].type, pDirEntry[i].inodeNum);
	}

	printf("============================\n");
	RemoveDir("/temp/aaa");
	CloseFileSystem();
	return 0;
}
 