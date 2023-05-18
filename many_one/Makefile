run:main
	./main

main:factorial.o manyone.o spinlock.o 
	gcc factorial.o manyone.o spinlock.o -o main 

factorial.o:factorial.c 
	gcc -c factorial.c -o factorial.o

manyone.o:manymany.c
	gcc -c manyone.c -o manyone.o

spinlock.o:spinlock.c
	gcc -c spinlock.c -o spinlock.o

clean:
	rm *.o
