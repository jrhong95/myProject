2015726096 : testcase.o disk.o fs.o fs_internal.o
	gcc -o 2015726096 testcase.o disk.o fs.o fs_internal.o
disk.o : disk.c
	gcc -c -o disk.o disk.c
fs.o : fs.c
	gcc -c -o fs.o fs.c
fs_internal.o : fs_internal.c
	gcc -c -o fs_internal.o fs_internal.c
clean :
	rm *.o 2015726096 MY_DISK
