/*
Problem 3
Generate a new process and output "StudentIDParent" with PID,then generates its child process, which output "StudentIDChild" with PID.
*/
/* use fork() and execl()*/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main()
{
    pid_t pid;
    printf("520030910306Parent %d\n",(int)getpid());
    pid = fork(); /* creating child process */

    if(pid<0)  /* fork failed */
    {
        fprintf(stderr, "Fork Failed");
        return 1;
    }

    else if(pid==0)  /* child process */
    {
        printf("520030910306Child %d\n",(int)getpid());
        execl("/data/misc/test_ptree","ptree",(char *) NULL); /* execute ptree */
    }

    else   /* parent process */
    {
        wait(NULL);
    }
    return 0;
}

