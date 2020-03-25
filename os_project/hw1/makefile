CC = gcc
TARGET = main

$(TARGET) : main.o hw1.o
	$(CC) -o $(TARGET) main.o hw1.o

main.o : main.c
	$(CC) -c -o main.o main.c

hw1.o : hw1.c
	$(CC) -c -o hw1.o hw1.c

clean : 
	rm *.o $(TARGET)