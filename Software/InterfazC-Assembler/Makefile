all:
	sudo as -o fahr_asm.o fahr_asm.s
	gcc -o conversion_temp conversion_temp.c fahr_asm.o
	sudo ./conversion_temp
clean:
	sudo rm conversion_temp
	sudo rm fahr_asm.o
