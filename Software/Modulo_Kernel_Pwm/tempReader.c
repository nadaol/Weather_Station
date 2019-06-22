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
void calcRgb(char*);
void refreshCpuUsage(void);

char dutyValues[30];
long double a[10], b[10],CpuPercentage;
FILE *fp;

/*creacion del thread para la lectura/escritura en background de la temperatura al modulo de kernel*/
int main()
{
	pid_t pid;
	pid = fork();
	if(pid==0)
	{
		while(1){
		refreshPwm();
	}
	}
	else if (pid<0) perror("Could't make new thread");
	return 0;
}
/*lectura del uso del cpu,escritura al fs del modulo*/
void refreshPwm()
{
int fd;
fd = open("/sys/pwm/pwm",O_RDWR);
if(fd==  -1 )
{
    printf("El archivo %s no existe o a sido bloqueado\n","/sys/pwm/pwm");
    exit(-1);
}
refreshCpuUsage();
calcRgb(dutyValues);
write(fd,dutyValues,sizeof(dutyValues));
}
//Calculo de intensidades rgb segun Uso del cpu
void calcRgb(char* rgb)
{
    float red=0;
    float green=255;
    double usage=CpuPercentage;
    while(usage>0)
    {
	red+=2.55;
	green-=2.55;
	usage--;
    }
    sprintf(rgb,"1 1000 %.0f %.0f 0",red,green);
    printf("Cpu : %.2Lf %\n",CpuPercentage);
}
//Lectura y calculo del uso del cpu
void refreshCpuUsage(){
        fp = fopen("/proc/stat","r");
        fscanf(fp,"%*s %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf",&a[0],&a[1],&a[2],&a[3],&a[4],&a[5],&a[6],&a[7],&a[8],&a[9]);
        fclose(fp);
	double CpuTimeSinceBootA=a[0]+a[1]+a[2]+a[3]+a[4]+a[5]+a[6];
	double CpuIdleTimeSinceBootA=a[3]+a[4];
	double CpuUsageSinceBootA=CpuTimeSinceBootA-CpuIdleTimeSinceBootA;
        sleep(1);
        fp = fopen("/proc/stat","r");
        fscanf(fp,"%*s %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf",&b[0],&b[1],&b[2],&b[3],&b[4],&b[5],&b[6],&b[7],&b[8],&b[9]);
        fclose(fp);
	double CpuTimeSinceBootB=b[0]+b[1]+b[2]+b[3]+b[4]+b[5]+b[6];
	double CpuIdleTimeSinceBootB=b[3]+b[4];
	double CpuUsageSinceBootB=CpuTimeSinceBootB-CpuIdleTimeSinceBootB;
	CpuPercentage=((CpuUsageSinceBootB-CpuUsageSinceBootA)*100)/(CpuTimeSinceBootB-CpuTimeSinceBootA);
}

