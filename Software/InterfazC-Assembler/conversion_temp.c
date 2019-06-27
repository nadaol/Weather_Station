#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int fahr_asm(int,int);

int fahrenheit(float celsius)
{
	/*  FAHRENHEIT = (CELSIUS * 1.8) + 32
	*	Aproximamos a:
	* 	F = (C*2) + 32
	*/
	int valor = (int) celsius;
	int mul = 2;
	valor = fahr_asm(valor,mul);
	
	return valor;
}

int main (void)
{
	float celsius = 20;
	printf( "Celsius: %.2f°C\n", celsius );
	int fahr = fahrenheit(celsius);
	printf( "Fahrenheit: %d °F\n", fahr );

	return 0;

}
