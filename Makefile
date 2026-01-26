all: folder chaos.o chaos.a

folder:
	mkdir -p ./build

chaos.o: chaos.h chaos.c folder
	gcc -c chaos.c -o ./build/chaos.o

chaos.a: folder
	ar rcs ./build/libchaos.a ./build/chaos.o  
	
