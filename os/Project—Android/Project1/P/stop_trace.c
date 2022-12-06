/*
This module stops tracing
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
#define _SYS_CALL_ 362
static int (*oldcall)(void);
void* sys_stop_trace(pid_t pid){

	struct pid *pidk = find_get_pid(pid);       /* get the information of kpid */
	struct task_struct *task = pid_task(pidk,PIDTYPE_PID);    /* get the information of task */
	task->trace_flag = 0;            /* change the trace state to 'stop trace' */   
	task->wcounts = 0;      
	printk(KERN_INFO"Stop trace\n");
}

static int addsyscall_init(void)
{
	long *syscall = (long*)0xc000d8c4;
	oldcall = (int(*)(void))(syscall[_SYS_CALL_]);
	syscall[_SYS_CALL_] = (unsigned long)sys_stop_trace;
	printk(KERN_INFO "module stop load!\n");
	return 0;
}
static void addsyscall_exit(void)
{
	long *syscall = (long*)0xc000d8c4;
	syscall[_SYS_CALL_] = (unsigned long)oldcall;
	printk(KERN_INFO "module stop exit!\n");

}

module_init(addsyscall_init);
module_exit(addsyscall_exit);

