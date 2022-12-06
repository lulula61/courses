/*
Problem 4: server side
The server can serve at most 2 clients concurrently, more clients coming have to wait.
basic idea: for each newly coming client,  allocate a thread for it. when a process sending message to server,
check whether it can be served, and return feedback.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define NUM_THREADS_ALL 2    // most number of clients can serve.

int num_thread=0;   // the number of all client
int num_working_thread=0; // the number of client in service

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // mutex to keep synchronization of global variable

void *server(void *newsockfd);
int main(int argc, char *argv[])
{   
    ////////////////////////
    // DO NOT CHANGE THIS PART  //
    int i,flag;
    int sockfd, newsockfd, portno, clilen, n, connectfd;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0)
    {
        printf("ERROR opening socket\n");
        exit(1);
    }
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // the port number (>2000 generally) of server is randomly assigned 
    portno = 2050;
    serv_addr.sin_port = htons(portno);
    // the ip address of server
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0) //binding
    {
        printf("ERROR binding\n");
        exit(1);
    }
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    printf("Server initiating...\n");

    // DO NOT CHANGE THIS PART /////////

    //////////////////////////////////////////////////

    while(1){  
        pthread_t tid;   // creating a new thread id

	//connect
        if( (connectfd = accept(sockfd, (struct sockaddr*)NULL, NULL)) == -1)
        {  
            printf("ERROR accepting\n");  
            continue;  
        }
        else
            num_thread++;

        // creating a thread
        if(pthread_create(&tid,NULL,server,(void *)(&connectfd)) == -1)
        {
            perror("ERROR thread creating");  
            close(connectfd);  
            close(sockfd); 
            exit(0); 
        }
    }  
    close(sockfd);   
    //////////////////////////////////////////////////
    return 0;
}
void *server(void *sockfd)
{

    int newsockfd = (int)(*((int*)sockfd));
    int n,i,lock=1; // deciding whether to encrypt for each thread 
    char buffer[256];

    ///////////////////////////////////////////

    //Finish your Encryption service here    //

    ///////////////////////////////////////////
    while(1)
    {
        if((n = recv(newsockfd, buffer, 256, 0)) < 0) // receiving
        {
            perror("ERROR receiving");  // exception case
            break;
        }
        buffer[n] = '\0';
        // input :q to end the service
        if(!strcmp(buffer, ":q"))     
        {
            printf("Server thread closing ...\n");
            break;
        }

        // to check the current whether current can be served
	// permit mutual exclusion 
        pthread_mutex_lock( &mutex );
        if(lock && num_working_thread<NUM_THREADS_ALL)
        {
            num_working_thread++;
            lock=0;
        }
        pthread_mutex_unlock( &mutex );

    
        if(!lock)
        {
            printf("Receiving message: %s\n", buffer); 

            // encrypting
            for(i=0; i<n; i++)
            {
                if( buffer[i] >= 'a' && buffer[i] <= 'z' )
                    buffer[i] = (buffer[i]-'a'+3)%26+'a';
                if(buffer[i]>='A'&&buffer[i]<='Z')
                    buffer[i] = (buffer[i]-'A'+3)%26+'A';
            }

            // sending
            if( send(newsockfd, buffer, strlen(buffer),0) < 0)
            {
                perror("ERROR sending");
                break;
            } 
        }
        else
        {
            // tell client to wait
            if( send(newsockfd, "Please wait ...", 16,0) < 0)
            {
                perror("ERROR sending");
                break;
            }
        }
    }
    close(newsockfd); 

    // release service place after finish service
    pthread_mutex_lock( &mutex );
    num_thread--;
    if(!lock)
        num_working_thread--;
    pthread_mutex_unlock( &mutex );

    pthread_exit(0);
}
