run:main
	./main

main: factorial.o manymany.o spinlock.o
	gcc factorial.o manymany.o spinlock.o -o main

factorial.o: factorial.c
	gcc -c factorial.c -o factorial.o

manymany.o: manymany.c
	gcc -c manymany.c -o manymany.o

spinlock.o: spinlock.c
	gcc -c spinlock.c -o spinlock.o

clean:
	rm *.o
