#include "queue.hpp"
#include <stdlib.h>



/**
 * @brief Create new queue - allocate memory and initialization of parameters
 * 
 * @return void* 
 */

void *createQ(){
	queueptr queue = (queueptr) malloc(sizeof(struct queue));
	if (queue == NULL){
		perror("malloc error");
		exit(0);
	}
	queue->end = NULL;
	queue->start = NULL;
	queue->size = 0;
	queue->cv = PTHREAD_COND_INITIALIZER;
    queue->lock = PTHREAD_MUTEX_INITIALIZER;

	return (void*) queue;
}



/**
 * @brief Enqueue fucntion into the queue - allocate memeory for node checking all conditions
 * 
 * @param queue 
 * @param new_key 
 */

void enQ(void *queue, void *new_key) {
	
	queueptr q = (queueptr) queue;
	pthread_mutex_lock(&(q->lock));
	nodeptr node = (nodeptr) malloc(sizeof(struct node));
	if (node == NULL) {
		exit(0);
	}
	node->key = new_key;
	node->next = NULL;
	if (q->size == 0) {
		q->end = node;
		q->start = node;
	}
	else {
		q->end->next = node;
		q->end = node;

	}
	pthread_cond_signal(&(q->cv));
	pthread_mutex_unlock(&(q->lock));
	q->size+=1;
	

}


/**
 * @brief Dequeue function of node from queue - free memory of node
 * 
 * @param queue 
 * @return void* 
 */

void *deQ(void *queue) {

	queueptr q = (queueptr) queue;
	pthread_mutex_lock(&(q->lock));
	if (q->size ==0){
		pthread_cond_wait(&(q->cv), &(q->lock));
	}
	nodeptr free_node = q->start;
	q->start = free_node->next;
	void *return_value = free_node->key;
	free(free_node);
	q->size-=1;
	pthread_mutex_unlock(&(q->lock));
	return return_value;
}


/**
 * @brief Destroy the queue by free all the nodes and the queue
 * 
 * @param queue 
 */

void destroyQ(void *queue) {
	queueptr q = (queueptr) queue;
	while (q->size > 0)
	{
		void * value = deQ(queue);
		free(value);
	}
	free(q);
}





