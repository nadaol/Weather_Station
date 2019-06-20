#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define MAXTIMINGS	85
#define DHTPIN		7                       // GPIO 4 Raspberry - pin 7 wiringPi
int dht11_dat[5] = { 0, 0, 0, 0, 0 };       //inicializo arreglo. (5 x 8 bits)
 
void read_dht11_dat()
{
	uint8_t laststate	= HIGH;             
	uint8_t counter		= 0;
	uint8_t j		= 0, i;
	float	f; 
 
	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;
 
	pinMode( DHTPIN, OUTPUT );              // GPIO como pin de salida
	digitalWrite( DHTPIN, LOW );            // Envio un "0" para inicio del sensor
	delay( 18 );                            // Espero 18 ms (tiempo recomendado para adquisicion de senal)
	digitalWrite( DHTPIN, HIGH );           // Envio un "1". Retomo estado de Raspi. 
	delayMicroseconds( 40 );                // Espero 40 us (tiempo recomendado de alto)
	pinMode( DHTPIN, INPUT );               // GPIO como pin de entrada 
 
    // detecto cambios y leo datos
	for ( i = 0; i < MAXTIMINGS; i++ )      
	{
		counter = 0;                        
		while ( digitalRead( DHTPIN ) == laststate )  // observo cambio de estado del pin      
		    {
		    	counter++;                  // si no cambio el dato, incremento contador
		    	delayMicroseconds( 1 );     // espero 1 ms
		    	if ( counter == 255 )       
		    	{
			    	break;                  // si el contador es 255, salgo.
		    	}
	    	}
		laststate = digitalRead( DHTPIN );      //leo el dato
 
		if ( counter == 255 )
			break;                     // si contador = 255, salgo
 
		if ( (i >= 4) && (i % 2 == 0) )     // ignoro las primeras 3 transiciones
		{
			dht11_dat[j / 8] << = 1;       // empujo el bit en cada posicion del arreglo de dato
			if ( counter > 50 )
				dht11_dat[j / 8] |= 1;
			j++;
		}
	}
 
    // realizo verificacion de los 40 bits y realizo checksum
	if ( (j >= 40) &&
	     (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) ) 
    	{
	    	f = dht11_dat[2] * 9. / 5. + 32;
	    	printf( "Humidity = %d.%d %% Temperature = %d.%d C (%.1f F)\n",
			dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f );
    	}
    else        // si los datos están corruptos, advierto por consola.
        {
		    printf( "Data not good, skip\n" );
	    }
}
 
int main( void )
{
	printf( "Raspberry Pi wiringPi DHT11 Temperature test program\n" );
 
	if ( wiringPiSetup() == -1 )        // verifico correcta carga de librería wiringPi
		exit( 1 );
 
	while ( 1 )
	{
		read_dht11_dat();               // llamo a funcion de lectura de datos del sensor
		delay( 1000 );                  // muestras se realizan cada 1 segundo
	}
 
	return(0);
}
