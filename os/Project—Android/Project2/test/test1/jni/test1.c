#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
 
static int alloc_size;
static char* memory;
static int times;
 
void segv_handler(int signal_number)
{
	printf("find memory accessed!\n");
	mprotect(memory, alloc_size, PROT_READ | PROT_WRITE);
	times++;
	printf("set memory read write!\n");
}
 
int main()
{
	int fd;
	struct sigaction sa;
    	unsigned long wcount = 0;
    	int res = 0;

    	printf("Start memory trace testing program!\n");

	syscall(361, getpid());
 
	/* Init segv_handler to handle SIGSEGV */
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &segv_handler;
	sigaction(SIGSEGV, &sa, NULL);

    	times = 0;
 
	 /* allocate memory for process, set the memory can only be read */
	alloc_size = 10 * getpagesize();
	fd = open("/dev/zero", O_RDONLY);
	memory = mmap(NULL, alloc_size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	/* try to write, will receive a SIGSEGV */
	memory[0] = 0;
	printf("memory[0] = %d\n", memory[0]);

	/* set protection */
	mprotect(memory, alloc_size, PROT_READ);
	/* try to write, will receive a SIGSEGV */
	memory[0] = 1;
	printf("memory[0] = %d\n", memory[0]);
	
	/*set protection */
	mprotect(memory, alloc_size, PROT_READ);
	/* try to write again, will receive a SIGSEGV */
	memory[1] = 2;
	printf("memory[1] = %d\n", memory[1]);
	
	mprotect(memory, alloc_size, PROT_READ);
	/* try to write, will receive a SIGSEGV */
	memory[2] = 3;
	printf("memory[2] = %d\n", memory[2]);	

	/* Get wcount */
	printf("The initial value of wcounts : %lu\n",wcount);
        res = syscall(363, getpid(), &wcount);
        // if(res) printf("error!!!\n");
        printf("Task pid : %d, Wcount = %lu, times = %d\n", getpid(), wcount, times);
        syscall(362, getpid());
	/* freee */
	munmap(memory, alloc_size);
	return 0;
}
