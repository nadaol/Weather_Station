#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

float fahrenheit(float celsius)
{
	float constante = 32;
	float fraccion = 1.8;
	
	float valor = (celsius*fraccion) + 32;
	
	return valor;
}

int main (void)
{
	float celsius = 20;
	//printf( "Celsius:%.2f°C\n", celsius );
	printf( "Celsius: %.2f°C\n", celsius );
	float fahr = fahrenheit(celsius);
	printf( "Fahrenheit: %.2f°F\n", fahr );

	return 0;

}
