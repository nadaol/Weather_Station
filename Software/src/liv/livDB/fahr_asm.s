@ fahr_asm.s
@ Descripcion: multiplica por 2 y suma la constante 32 
@	       para lograr temp Fahrenheit

@ Info de Raspberry
        .cpu    cortex-a53
        .fpu    neon-fp-armv8
        .syntax unified         

@ Datos
	
	.data
	.align  4	
cons:	.word	32		@defino variable cons valor 32 dec 

@ Codigo
        .text
        .align  4
        .global fahr_asm
        .type   fahr_asm, %function
	
fahr_asm:
        str     fp, [sp, #-4]!  @ Salvo contexto - guardo direccion de retorno 
        add     fp, sp, #0      @ nuevo frame pointer = stack pointer

	sub	sp, sp, #12	@ espacio para variables locales
	str	r0, [fp, #-8]	@ guardo el primer parametro en r0 
	str	r1, [fp, #-12]	@ guardo el segundo parametro en r1 
	
	ldr	r2, [fp, #-8]	@ cargo en r2 como una variable local
				@ el valor del primer parametro

	ldr 	r3, [fp, #-12]	@ cargo en r3 como una variable local
				@ el valor del segundo parametro

	mul	r3, r2, r3	@ multiplico r2 y r3 y lo guardo en r3

	ldr	r2, =cons	@ guardo en r2 la direccion de cons
        ldr	r2,[r2]		@ guardo en r2 el valor almacenado en r2
				
	add	r3, r2, r3	@ sumo r2 a r3 y guardo en r3

	mov	r0, r3		@ el valor de r3 lo almaceno en r0

	add	sp, fp, #0	@ retorno el valor original del frame pointer 
				@ stack pointer = frame pointer

        ldr     fp, [sp], #4    @ cargo direccion de retorno y quito parametros de la pila

	bx      lr              @ retorno
