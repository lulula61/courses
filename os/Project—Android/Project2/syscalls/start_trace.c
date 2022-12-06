/*
This module tells the kernel to start tracing page writes for the process pid.
*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/unistd.h>
#include <linux/list.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
MODULE_LICENSE("Dual BSD/GPL");
#define _SYS_CALL_ 361
static int (*oldcall)(void);
void* sys_start_trace(pid_t pid){
	
	struct pid *pidk = find_get_pid(pid);       /* get the information of kpid */
	struct task_struct *task = pid_task(pidk,PIDTYPE_PID);    /* get the information of task */
	task->wcounts = 0;        /* initialize wconuts */
	task->trace_flag = 1;            /* change the trace state to 'start trace' */         
	
	printk(KERN_INFO"START TRACING : %d\n",task->pid);
}

static int addsyscall_init(void)
{
	long *syscall = (long*)0xc000d8c4;
	oldcall = (int(*)(void))(syscall[_SYS_CALL_]);
	syscall[_SYS_CALL_] = (unsigned long)sys_start_trace;
	printk(KERN_INFO "module start load!\n");
	return 0;
}
static void addsyscall_exit(void)
{
	long *syscall = (long*)0xc000d8c4;
	syscall[_SYS_CALL_] = (unsigned long)oldcall;
	printk(KERN_INFO "module start exit!\n");

}

module_init(addsyscall_init);
module_exit(addsyscall_exit);

