all:
	sudo as -o fahr_asm.o fahr_asm.s
	gcc -shared -o libpyc.so -fPIC conversion_temp.c fahr_asm.o
	sudo python livDB.py
clean:
	sudo rm fahr_asm.o
	sudo rm libpyc.so