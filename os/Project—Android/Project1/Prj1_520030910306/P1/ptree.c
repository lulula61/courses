/*This module write a new system call in Android, it takes two arguments and return the process tree information in a depth-first-search order.*/
// the system call is 356


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/unistd.h>
#include <linux/list.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
MODULE_LICENSE("Dual BSD/GPL");
#define _syscall_ptree 356
#define BUFFERSIZE 2048
static int (*oldcall)(void);
struct prinfo {
	pid_t parent_pid; /* process id of parent */
	pid_t pid; /* process id */
	pid_t first_child_pid; /* pid of youngest child */
	pid_t next_sibling_pid; /* pid of older sibling */
	long state; /* current state of process */
	long uid; /* user id of process owner */
	char comm[16]; /* name of program executed */
};


void change(struct task_struct* a, struct prinfo* b)       //give the new prinfo information
{
	if(a->parent)
		b->parent_pid = a->parent->pid;
	else b->parent_pid = 0;

	b->pid = a->pid;

	if(list_empty(&(a->children)))
		b->first_child_pid = 0;
	else b->first_child_pid = list_entry(a->children.next, struct task_struct, sibling)->pid;
	
	if(list_empty(&(a->sibling)))
		b->next_sibling_pid = 0;
	else b->next_sibling_pid = list_entry(a->sibling.next, struct task_struct, sibling)->pid;
	
	b->state = a->state;
	
	b->uid = a->cred->uid;
	
	get_task_comm(b->comm, a);
}

void dfs(struct task_struct* ktask, struct prinfo* ptask, int* cnt)
{
	struct list_head *head;
	struct task_struct *task;
	int i = *cnt;
	change(ktask, &ptask[i]);
	(*cnt)++;
	
	list_for_each(head, &(ktask->children))
	{
		task = list_entry(head, struct task_struct, sibling);
		dfs(task, ptask, cnt);
	}
}


int ptree(struct prinfo *buf,int *nr)
{
	struct prinfo *buffer = (struct prinfo*)kmalloc(sizeof(struct prinfo)* BUFFERSIZE, GFP_KERNEL);
	int cnt = 0;
	
	read_lock(&tasklist_lock);
	dfs(&init_task, buffer, &cnt);
	read_unlock(&tasklist_lock);

	//buffer-> buff     cnt->nr
	if(copy_to_user(buf, buffer, sizeof(struct prinfo)* BUFFERSIZE) || copy_to_user(nr, &cnt, sizeof(int)))
	{
		printk(KERN_INFO "COPY FAILED\n");
		return 1;
	}

	kfree(buffer);
	return 0;
}
static int addsyscall_init(void)
{
	long *syscall = (long*)0xc000d8c4;
	oldcall = (int(*)(void))(syscall[_syscall_ptree]);
	syscall[_syscall_ptree] = (unsigned long)ptree;
	printk(KERN_INFO "module load!\n");
	return 0;
}
static void addsyscall_exit(void)
{
	long *syscall = (long*)0xc000d8c4;
	syscall[_syscall_ptree] = (unsigned long)oldcall;
	printk(KERN_INFO "module exit!\n");

}

module_init(addsyscall_init);
module_exit(addsyscall_exit);

