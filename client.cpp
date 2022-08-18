/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to

#define MAXDATASIZE 1024 // max number of bytes we can get at once

int i;
int flag;
// void *listen_for_msg(void *args)
// {

// 	int sockfd = *(int *)args;
// 	char buf[MAXDATASIZE];

// 	while (flag == 1)
// 	{
// 		memset(buf, '\0', MAXDATASIZE);
// 		int t = recv(sockfd, buf, MAXDATASIZE, 0);
// 		if (t > 0)
// 		{
// 			printf("%s\n", buf);
// 			// fflush(stdout);
// 		}
// 		else if (t < 0)
// 		{
// 			printf("recv<0\n");
// 			perror("recv");
// 		}
// 	}

// 	close(sockfd);

// 	pthread_exit(&i);
// }

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in *)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
	int sockfd, numbytes;
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];
	flag = 1;
	if (argc != 2)
	{
		fprintf(stderr, "usage: client hostname\n");
		exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
							 p->ai_protocol)) == -1)
		{
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			perror("client: connect");
			close(sockfd);
			continue;
		}

		break;
	}

	if (p == NULL)
	{
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			  s, sizeof s);
	printf("client: connecting to %s\n", s);


	//Create listening thread 
	// pthread_t id;
	// int ret;
	// ret = pthread_create(&id, NULL, &listen_for_msg, &sockfd);

	freeaddrinfo(servinfo); // all done with this structure

	//User input loop
	char input[MAXDATASIZE]; // input text capped at 1024, and "PUSH " size is 5 + \0
	while (1)
	{
		memset(input, '\0', MAXDATASIZE);
		scanf(" %[^\n]s", input);
		   int bytes=0;
        if((bytes = send(sockfd,input,strlen(input)+1,0))==-1){//SEND COMMAND TO SERVER.
            perror("send");
            exit(1);
        }
        printf("client: sent %d bytes\n",bytes);
        if(1){//IF COMMAND IS TOP EXPECTING MESSAGE FROM SERVER TO OUTPUT.
              if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
                perror("recv");
                exit(1);
                }
            buf[numbytes] = '\0';
            if(numbytes > 0){
                printf("OUTPUT '%s'\n",buf); 
            }
        }
	}

	// if ((numbytes = recv(sockfd, buf, MAXDATASIZE - 1, 0)) == -1)
	// {
	// 	perror("recv");
	// 	exit(1);
	// }

	// buf[numbytes] = '\0';

	// printf("client: received '%s'\n", buf);

	close(sockfd);

	return 0;
}
