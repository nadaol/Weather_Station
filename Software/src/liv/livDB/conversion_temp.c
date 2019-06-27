#include <stdio.h>
#include <stdlib.h>
#include "conversion_temp.h"

int fahr_asm(int,int);

void connect()
{
    //printf("Connected to C extension...\n");
}

int fahrenheit(float celsius)
{
	/*  FAHRENHEIT = (CELSIUS * 1.8) + 32
	*	Aproximamos a:
	* 	F = (C*2) + 32
	*/
	
	//printf( "C -- Celsius: %.2f°C\n", celsius );

	
	int valor = (int) celsius;
	int mul = 2;
	valor = fahr_asm(valor,mul);
	
	//printf( "C -- Fahrenheit: %d °F\n", valor );

	
	return valor;
}

