#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/time.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>

MODULE_DESCRIPTION("Monitor de temperatura pwm");
MODULE_LICENSE("GPL");
#define DEVICE_NAME "pwmDriverPin18"	
MODULE_AUTHOR("Nadaol");
 
/* GPIO*/
unsigned const int PinRed = 17;
unsigned const int PinGreen = 27;
unsigned const int PinBlue = 22;
 /*Variables de control*/
unsigned static int RedDuty=0;		// Dutycycle [%](Lectura por medio del fs sys/pwm/pwm)
unsigned static int GreenDuty=0;	//valores iniciales
unsigned static int BlueDuty=5;
unsigned static int RedOn;
unsigned static int GreenOn;
unsigned static int BlueOn;
unsigned static int RedOff;
unsigned static int GreenOff;
unsigned static int BlueOff;
unsigned static int freq 		= 1;		// Frecuencia [Hz] (Lectura por medio del fs sys/pwm/pwm)
unsigned static int enable		= 1;		//Enable pwm ->1 Habilitado
static void calcPeriodos(void);


/*----------------------------------------------*/


/*Configuracion GPIO*/
int pwm_gpio_init(void)
{
	printk(KERN_INFO "Started GPIO initialization in %s\n", __FUNCTION__);
	gpio_request(PinRed, "Red");
	gpio_request(PinGreen, "Green");
	gpio_request(PinBlue, "Blue");
	gpio_direction_output(PinRed, 0);
	gpio_direction_output(PinGreen, 0);
	gpio_direction_output(PinBlue, 0);
	return 0;
}
 
void pwm_gpio_exit(void)
{
	printk(KERN_INFO "Releasing GPIO ports in %s \n", __FUNCTION__);
	gpio_free(PinRed);
	gpio_free(PinGreen);
	gpio_free(PinBlue);
}
/*----------------------------------------------*/

/*Funciones para el pwm (ejecutada en un segundo thread)*/
#define THREAD_NAME0 "pwmRedPin"	// Nombre del hilo
struct task_struct *task0;		//tarea
/* Tarea del thread*/
int pwm_Run_Red(void *data) 
{
	while(1)  {
		if(enable && RedDuty!=0){/*Generacion del pwm*/
			gpio_set_value(PinRed,0);
			if(RedOn>10000)msleep(RedOn/1000);
			else usleep_range(RedOn,RedOn);
			gpio_set_value(PinRed,1);
			if(RedOff>10000)msleep(RedOff/1000);
			else usleep_range(RedOff,RedOff);
		} else {
			gpio_set_value(PinRed,1);
			msleep(200);
		}
		if (kthread_should_stop()) break;//Si se produce un error,detengo el hilo
	}
	return 0;
}
#define THREAD_NAME1 "pwmGreenPin"	// Nombre del hilo
struct task_struct *task1;		//tarea
int pwm_Run_Green(void *data) 
{
	printk(KERN_INFO "Starting Green: Pin %d Duty %d On %d Off %d",PinGreen,GreenDuty,GreenOn,GreenOff);
	while(1)  {
		if(enable && GreenDuty!=0){/*Generacion del pwm*/
			gpio_set_value(PinGreen, 0);
			if(GreenOn>10000)msleep(GreenOn/1000);
			else usleep_range(GreenOn,GreenOn);
			gpio_set_value(PinGreen, 1);
			if(GreenOff>10000)msleep(GreenOff/1000);
			else usleep_range(GreenOff,GreenOff);
		} else {
			gpio_set_value(PinGreen,1);
			msleep(200);
		}
		if (kthread_should_stop()) break;//Si se produce un error,detengo el hilo
	}
	return 0;
}
#define THREAD_NAME2 "pwmBluePin"	// Nombre del hilo
struct task_struct *task2;		//tarea
int pwm_Run_Blue(void *data) 
{
	while(1)  {
		if(enable && BlueDuty!=0){/*Generacion del pwm*/
			gpio_set_value(PinBlue, 0);
			if(BlueOn>10000)msleep(BlueOn/1000);
			else usleep_range(BlueOn,BlueOn);
			gpio_set_value(PinBlue, 1);
			if(BlueOff>10000)msleep(BlueOff/1000);
			else usleep_range(BlueOff,BlueOff);
		} else {
			gpio_set_value(PinBlue,1);
			msleep(200);
		}
		if (kthread_should_stop()) break;//Si se produce un error,detengo el hilo
	}
	return 0;
}
/* funcion de inicializacion del hilo */
int pwm_runThread_init(void)
{
	calcPeriodos();
	printk(KERN_ALERT "STARTING PWM");
	task0 = kthread_run(pwm_Run_Red, NULL, THREAD_NAME0);
	task1 = kthread_run(pwm_Run_Green, NULL, THREAD_NAME1);
	task2 = kthread_run(pwm_Run_Blue, NULL, THREAD_NAME2);
	return 0;
}
/* Función de finalizacion del hilo */
void pwm_runThread_exit(void)
{
	printk(KERN_ALERT "STOPPING PWM in %s.\n", __FUNCTION__);
	kthread_stop(task0);
	kthread_stop(task1);
	kthread_stop(task2);
}
/*----------------------------------------------*/

/* Funciones para la comunicacion con el espacio de usuario */
static struct kobject *pwm_kobject;
 
static ssize_t set_pwm(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count)
{
	sscanf(buff, "%d %d %d %d %d", &enable, &freq, &RedDuty,&GreenDuty,&BlueDuty);
	RedDuty=RedDuty*100/255;
	GreenDuty=GreenDuty*100/255;
	BlueDuty=BlueDuty*100/255;
	calcPeriodos();
	printk(KERN_INFO "Read values: %d %d %d %d %d", enable, freq, RedDuty,GreenDuty,BlueDuty);
	/* Calculo tiempos para generar el pwm con el duty cycle y freq leido del fs sys/pwm/pwm*/

		
	return count;
}
static void calcPeriodos()
{
RedOn =((1000000/freq)*RedDuty)/100;// Duration of ON-Period
GreenOn =((1000000/freq)*GreenDuty)/100;	
BlueOn =((1000000/freq)*BlueDuty)/100;	
RedOff = ((1000000/freq)*(100-RedDuty))/100;// Duration of OFF-Period
GreenOff = ((1000000/freq)*(100-GreenDuty))/100;
BlueOff = ((1000000/freq)*(100-BlueDuty))/100;
}
 
static struct kobj_attribute pwm_attribute = __ATTR(pwm, (S_IWUSR | S_IRUGO), NULL, set_pwm);
 /*Creacion del fs*/
int pwm_sysfs_init(void) 
{
	printk(KERN_INFO "PWM: starting sysfs in %s\n", __FUNCTION__);
	pwm_kobject = kobject_create_and_add("pwm", NULL);
	if(sysfs_create_file(pwm_kobject, &pwm_attribute.attr)) {
		pr_debug("failed to create pwm sysfs!\n");
	}
	
	return 0;
}


 /*Finalizacion del fs*/
void pwm_sysfs_exit(void)
{
	printk(KERN_INFO "PWM: stopping sysfs in %s\n", __FUNCTION__);
	kobject_put(pwm_kobject);
}
/*----------------------------------------------*/

/*Funcion de inicializacion general*/
int pwm_init(void){
	printk(KERN_INFO "\nLoading module %s\n", __FUNCTION__);
	pwm_gpio_init();
	pwm_sysfs_init();
	pwm_runThread_init();
	return 0;
}


 /*Funcion de finalizacion general*/
void pwm_exit(void){
	printk(KERN_INFO "\nExiting kernel in %s\n", __FUNCTION__);
	pwm_runThread_exit();
	pwm_sysfs_exit();
	pwm_gpio_exit();
}

 /*asigno funciones init,exit*/
module_init(pwm_init);
module_exit(pwm_exit);
