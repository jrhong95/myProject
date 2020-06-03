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
	CreateFileSystem();

	MakeDir("/temp");
	MakeDir("/temp/aaa");
	MakeDir("/tttt");
	CreateFile("/temp/abc");
	CreateFile("/temp/abc/aaa");

	CloseFile(0);
	OpenFile("/temp/abc");

	EnumerateDirStatus("/temp/aaa", pDirEntry, 20);
	printf("Fin\n");

	CloseFileSystem();
	return 0;
}
 