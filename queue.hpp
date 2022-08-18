#include <stdio.h>
#include <pthread.h>


/*********** stuct node **********/
typedef struct node{
    void *key;
    struct node *next;
} *nodeptr;


/*********** stuct queue **********/
typedef struct queue{
    pthread_cond_t cv;
    pthread_mutex_t lock;
    int size;
    nodeptr start;
    nodeptr end;
} *queueptr;


/*********** queue functions **********/
void *createQ();
void destroyQ(void *queue);
void enQ(void *queue, void *new_key);
void *deQ(void *queue);
