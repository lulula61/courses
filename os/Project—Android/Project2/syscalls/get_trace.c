/*
This module will get the wcounts of the task and return
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
#define _SYS_CALL_ 363
static int (*oldcall)(void);
void* sys_get_trace(pid_t pid, int *wcounts){
	
	struct pid *pidk = find_get_pid(pid);       /* get the information of kpid */
	struct task_struct *task = pid_task(pidk,PIDTYPE_PID);    /* get the information of task */
	int kwcount = task->wcounts;
	printk(KERN_INFO "GET TRACE !\n");
	if(put_user(kwcount, wcounts) == -EFAULT){
		printk(KERN_INFO "Error copying to user!\n");
		return -EFAULT;
	}
	else printk(KERN_INFO "the value of wcounts: %d\n", *wcounts);
}

static int addsyscall_init(void)
{
	long *syscall = (long*)0xc000d8c4;
	oldcall = (int(*)(void))(syscall[_SYS_CALL_]);
	syscall[_SYS_CALL_] = (unsigned long)sys_get_trace;
	printk(KERN_INFO "module get load!\n");
	return 0;
}
static void addsyscall_exit(void)
{
	long *syscall = (long*)0xc000d8c4;
	syscall[_SYS_CALL_] = (unsigned long)oldcall;
	printk(KERN_INFO "module get exit!\n");

}

module_init(addsyscall_init);
module_exit(addsyscall_exit);

