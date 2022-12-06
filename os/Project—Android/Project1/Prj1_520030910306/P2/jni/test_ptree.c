/* This mudole id to test new system call, print_tree() will print the dfs tree using tabs to indent children with respect to their parents */ 

#include <string.h>
#include <stdlib.h>
#include "stdio.h"
#define BUFFERSIZE 2048

struct prinfo
{
	pid_t parent_pid;
	pid_t pid;
	pid_t first_child_pid;
	pid_t next_sibling_pid;
	long state;
	long uid;
	char comm[16];
};

void print_tree(struct prinfo* buf, int *nr){

	int record[256];   
	int tab[256];
	memset(tab, 0, 256 * sizeof(int));
	memset(record, 0, 256 * sizeof(int));

	int  k = 0;     //tabs of each process
	int i, j;
	for(i = 0; i < *nr; ++i)
	{
		if(i == *nr-1) // To check if some next_sibling_pid refer to the root parent
		 	buf[i].next_sibling_pid = 0;
		
		if(buf[i].next_sibling_pid != buf[i+1].pid && buf[i].first_child_pid != buf[i+1].pid)
			buf[i].next_sibling_pid = 0;
			
		if( k == 0 || buf[i].pid == buf[record[k-1]].first_child_pid )      //the pre-process is the parent of this process
			record[k++] = i;
		else 
		{
			while(buf[i].pid != buf[record[k-1]].next_sibling_pid)            //find it's brother
			{
				buf[record[k-1]].next_sibling_pid = 0;
				k--;
			}
			record[k-1] = i;
		}	
		tab[i] = k-1;
	}
	
	printf("The number of the process: %d\n",*nr);

	for(i = 0; i < *nr; ++i)
	{
		for(j = 0; j < tab[i]; ++j)   //print tabs
			printf("\t");
		printf("%s,%d,%ld,%d,%d,%d,%ld\n", buf[i].comm, buf[i].pid, buf[i].state, buf[i].parent_pid, buf[i].first_child_pid, buf[i].next_sibling_pid, buf[i].uid);
	}

}


int main(int argc, char *argv[])
{
	struct prinfo *buf = (struct prinfo*)malloc(sizeof(struct prinfo) *BUFFERSIZE);
	int nr;
	syscall(356, buf, &nr);
	print_tree(buf, &nr);
	free(buf);
	return 0;
}
