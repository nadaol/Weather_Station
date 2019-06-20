#include <linux/init.h>
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
#define pwm1 	18 // Kernel PWM channel 0 on GPIO18
 
unsigned int duty 		= 50;		// Dutycycle [%](Lectura por medio del fs sys/pwm/pwm)
unsigned int Period 	= 2;		// Periodo [ms] (Lectura por medio del fs sys/pwm/pwm)
unsigned int enable		= 1;		//Enable pwm ->1 Habilitado
unsigned int tmsec_On	= 1;		//Tiempo On [ms]
unsigned int tmsec_Off	= 1;		//Tiempo Off [ms]
/*----------------------------------------------*/


/*Configuracion GPIO*/
int pwm_gpio_init(void)
{
	printk(KERN_INFO "Started GPIO initialization in %s\n", __FUNCTION__);
	gpio_request(pwm1, "pwm1");
	gpio_direction_output(pwm1, 0);
	
	return 0;
}
 
void pwm_gpio_exit(void)
{
	printk(KERN_INFO "Releasing GPIO ports in %s \n", __FUNCTION__);
	gpio_free(pwm1);
}
/*----------------------------------------------*/

/*Funciones para el pwm (ejecutada en un segundo thread)*/
#define THREAD_NAME0 "pwmPin"	// Nombre del hilo
struct task_struct *task0;		//tarea
/* Tarea del thread*/
int pwm_threadRun(void *data) 
{
	while(1)  {
		if(enable){/*Generacion del pwm*/
			gpio_set_value(pwm1, 1);
			msleep(tmsec_On);
			gpio_set_value(pwm1, 0);
			msleep(tmsec_Off);
		} else {
			gpio_set_value(pwm1,0);
			msleep(5000);
		}
		if (kthread_should_stop()) break;//Si se produce un error,detengo el hilo
	}
	return 0;
}
/* funcion de inicializacion del hilo */
int pwm_runThread_init(void)
{
	printk(KERN_ALERT "STARTING PWM: Period is %d milliseconds, and dutycycle is %d percent.\n", Period, duty);
	task0 = kthread_run(pwm_threadRun, NULL, THREAD_NAME0);
	return 0;
}
/* Función de finalizacion del hilo */
void pwm_runThread_exit(void)
{
	printk(KERN_ALERT "STOPPING PWM in %s.\n", __FUNCTION__);
	kthread_stop(task0);
}
/*----------------------------------------------*/

/* Funciones para la comunicacion con el espacio de usuario */
static struct kobject *pwm_kobject;
 
static ssize_t set_pwm(struct kobject *kobj, struct kobj_attribute *attr, const char *buff, size_t count)
{
	sscanf(buff, "%d %d %d", &enable, &Period, &duty);
	printk(KERN_INFO "Read values: %d %d %d", enable, Period, duty);
	/* Calculo tiempos para generar el pwm con el duty cycle leido del fs sys/pwm/pwm*/
	if(enable)  {
		tmsec_On = (Period*duty)/100;	// Duration of on-cycle...
		tmsec_Off = Period-tmsec_On;	// Duration of off-cycle...
	}
	return count;
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
