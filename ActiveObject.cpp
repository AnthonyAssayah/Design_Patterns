#include "ActiveObject.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h> 
#include <ctype.h>

#define PORT "3490" // the port users will be connecting to

#define BACKLOG 10 // how many pending connections queue will hold

queue* first_queue = NULL;
queue* second_queue = NULL;
queue* third_queue = NULL;
ActiveObject* AO1;
ActiveObject* AO2; 
ActiveObject* AO3; 


AO_ptr newAO(struct queue *q, void* (*f_func)(void*), void* (*q_func)(void*)) {

	AO_ptr my_AO = (AO_ptr) malloc(sizeof(struct ActiveObject));
    my_AO->queue = q;
    my_AO->first_func =  f_func;
    my_AO->second_func = q_func;
    pthread_create(&my_AO->my_thread, NULL, thread_func, my_AO);
    return my_AO;
    
}

void *thread_func(void *my_AO) {
    AO_ptr new_AO = (AO_ptr)my_AO;
    void* message;
    while(true) {
        message = deQ(new_AO->queue);
        printf("jjjjjjj%s\n", (char*)message);

        if (new_AO->first_func != NULL) {
            new_AO->first_func(message);
        }
        if (new_AO->second_func != NULL) {
            printf("fffff %s\n", (char*)message);
            new_AO->second_func(message);
        }
    }
    
}


void destroyAO(ActiveObject *my_AO) {

    destroyQ(my_AO->queue);
    free(my_AO);
}


void* first_Enqueue(void* message) {
    enQ(message, first_queue);
    return message;
}


void* ceasar(void* my_data){

    struct message *ceasar_mess = (struct message*)my_data;

    if (my_data == NULL){
        printf("The node data is empty!");
        return NULL;
    
    }
    
    int j = 0;
    while (j < strlen(ceasar_mess->text)) {


        if (ceasar_mess->text[j] >= 65 && ceasar_mess->text[j] <= 90) {
            ceasar_mess->text[j]  = (ceasar_mess->text[j]-64)%26+ 'A';
        }
        else if (ceasar_mess->text[j] >= 97 && ceasar_mess->text[j]  <= 122) {
             ceasar_mess->text[j]  = (ceasar_mess->text[j]-96)%26+ 'a';
        } 
       
        j++;  
    }
    return my_data;
}

void* second_Enqueue(void* message) {
    printf("message = %s\n", (char*)message);
    enQ(message, second_queue);
    printf("after enq\n");
    void* ret = deQ(second_queue);
    printf("after deq\n");

    printf("ret = %s\n", (char*)ret);
    return message;
}


void* convert_letters(void* my_data) {

    printf("my data = %s\n", (char*)my_data);

    struct message *conver_mess = (struct message*)my_data;

    if (my_data == NULL){
        printf("The node data is empty!");
        return NULL;
    }
   
    printf("the first char is = %c\n",conver_mess->text[0]);
    int j = 0;
    while (j < strlen(conver_mess->text)) {
        printf("the char #%d is = %c\n", j, conver_mess->text[j]);

        if (conver_mess->text[j] >= 65 && conver_mess->text[j] <= 90) {
            conver_mess->text[j]  = (conver_mess->text[j]  - 65) + 97;
        }
        else if (conver_mess->text[j] >= 97 && conver_mess->text[j]  <= 122) {
            conver_mess->text[j]  = (conver_mess->text[j]  - 97) + 65;
        } 
       
        j++;  
    }
    return my_data;

 }


void* third_Enqueue(void* message) {
    enQ(message, third_queue);
    return message;
}

void* to_client(void* my_data) {

    struct message *send_mess = (struct message*)my_data;

    if (my_data == NULL){
        printf("The node data is empty!");
        return NULL;
    }
    if (send(send_mess->fd,send_mess->text,strlen(send_mess->text),0) == -1) {
        printf("Error");
    }
    printf("sent: %s to file descriptor: %d\n", send_mess->text,send_mess->fd);
    // curr_mess->level = 3;
    return my_data;
}



 
void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void *connection_handler(void *client_ptr)
{
    int new_fd = *((int*)client_ptr);
    // free(client_ptr); 

    char buffer[1024];
    struct message* new_message;
    while(1){
        // char* buffer=(char*)malloc(sizeof(char)*1024);
        // bzero(buffer,1024);
        printf("Waiting for client to send data fd: %d\n",new_fd);
        int n = recv(new_fd,buffer,1024,0);
 
        new_message = (struct message*)malloc(sizeof(struct message));
        new_message->fd = new_fd;
        memcpy(new_message->text,buffer, sizeof(buffer));
        printf("Message text = '' %s '\n", new_message->text);
        enQ(first_queue,buffer);
        // new_message->fd = new_fd;
      
        // AO1 = newAO(first_queue,ceasar,second_Enqueue);
        // AO2 = newAO(second_queue,convert_letters,third_Enqueue);
        // AO3 = newAO(third_queue,to_client,NULL);
            
    }
    // destroyAO(AO1);
    // destroyAO(AO2);
    // destroyAO(AO3);
    // free(new_message);
    close(new_fd);
}


int main(void)
{


    // init our mutex lock
    first_queue = (queue*)createQ(); 
    second_queue = (queue*)createQ();
    third_queue = (queue*)createQ();
         
    AO1 = newAO(first_queue,ceasar,second_Enqueue);
    AO2 = newAO(second_queue,convert_letters,third_Enqueue);
    AO3 = newAO(third_queue,to_client,NULL);
    // AO1 = newAO(first_queue,ceasar,second_Enqueue);
    // AO2 = newAO(second_queue,convert_letters,third_Enqueue);
    // AO3 = newAO(third_queue,to_client,NULL);
    int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");
    int flag = 1;
    while (flag == 1)
    { // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

        
        if (new_fd == -1)
        {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);
        printf("File Descriptor = %d\n",new_fd);

        pthread_t id;
        int *client_ptr = (int*)malloc(sizeof(int));
        *client_ptr = new_fd; 
        pthread_create(&id,NULL,connection_handler,client_ptr);
    }

    destroyQ(first_queue);
    destroyQ(second_queue);
    destroyQ(third_queue);

    destroyAO(AO1);
    destroyAO(AO2);
    destroyAO(AO3);
    
    return 0;
}