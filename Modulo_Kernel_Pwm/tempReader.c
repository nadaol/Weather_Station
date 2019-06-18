#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

char* formatTemp(char*);
char* searchLn(char*,char*);
void printTemp(char*);
char* getTemp(void);
void refreshPwm(void);
unsigned int OnTemp=80;
unsigned int OffTemp=30;

/*creacion del thread para la lectura/escritura en background de la temperatura al modulo de kernel*/
int main()
{
	pid_t pid;
	pid = fork();
	if(pid==0)
	{
		while(1){//Lectura de la temperatura cada 5 segundos.
		refreshPwm();
		sleep(5);
	}
	}
	else if (pid<0) perror("Could't make new thread");
	return 0;
}
/*lectura y parseo de temperatura ,escritura al fs del modulo*/
void refreshPwm()
{
int fd;
fd = open("/sys/pwm/pwm",O_RDWR);
if(fd==  -1 )
{
    printf("El archivo %s no existe o a sido bloqueado\n","/sys/pwm/pwm");
    exit(-1);
}
char* temp=getTemp();
int miliTempC=atoi(temp);
unsigned int duty;
if(miliTempC>OffTemp) duty = (((miliTempC-(OffTemp*1000))*100)/((OnTemp-OffTemp)*1000));
else duty = 0;
float TempC = (float)miliTempC/(float)1000.00;
char pwm [20];
char Temp[5];
sprintf(Temp,"%.2f\n",TempC);
sprintf(pwm,"1 1000 %d",duty);
write(fd,pwm,sizeof(pwm));
}
/*Funcion para obtener la temperatura de la rspi en miliC*/
char* getTemp()//mili C°
{
	return searchLn("first","/sys/class/thermal/thermal_zone0/temp");
}

char* searchLn(char* search, char* fileDir)//Devuelve información contenida en el archivo dentro de fileDir que empiece con la palabra search.
{
		FILE * file;
	    char* line = (char *) malloc(sizeof(char) * 64);

	    file = fopen(fileDir, "r");

	    if (NULL == file)
	    {
	         fprintf(stderr,"File %s is invalid or doesn't exist",fileDir);
	    }
	    if(strcmp(search,"first")!=0)//en caso que se especifique el parametro search que no sea first (primer línea)
	    {
	    	while(strncmp(search,line,strlen(search)) != 0)
	    		    {
	    		    	fscanf(file, "%64[^\n]\n", line);//leer hasta encontrar la linea que comience con search
	    		    }
	    }
	    else fscanf(file, "%64[^\n]\n", line);

	    fclose(file);
	    return line;
}
