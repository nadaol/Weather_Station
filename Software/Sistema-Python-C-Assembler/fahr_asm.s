@ fahr_asm.s
@ Descripcion: suma la constante 32 para lograr temp Fahrenheit

@ Info de Raspberry
        .cpu    cortex-a53
        .fpu    neon-fp-armv8
        .syntax unified         

@ Datos
	
	.data
	.align  2	
cons:	.word	32		@defino variable cons valor 32 dec 

@ Codigo
        .text
        .align  2
        .global fahr_asm
        .type   fahr_asm, %function
	
fahr_asm:
        str     fp, [sp, -4]!   @ Salvo contexto 
        add     fp, sp, 0       @ reubico frame pointer

	mov	r2, #0		@ vacio el registro r2
	add 	r2,r1		@ agrego el valor del 2do parametro mul

	mul	r0,r2		@ multiplico a r0 el valor de r2

	ldr	r1, =cons	@ guardo en r1 la direccion de cons
        ldr	r1,[r1]		@ guardo en r1 el valor almacenado en r1
				@ r0 contiene el argumento pasado por C
	add	r0,r1		@ sumo r1 a r0 y guardo en r0

        sub     sp, fp, 0	@ devuelvo valor a stack pointer 
        ldr     fp, [sp], 4     @ devuelvo el valor de frame pointer de llamada
        bx      lr              @ retorno