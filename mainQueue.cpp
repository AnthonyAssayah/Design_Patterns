/************************* TESTING QUEUE *****************************/

#include <iostream>
#include "queue.hpp"


using namespace std;


int main()
{

	// test queue's functions
	queueptr q = (queueptr) createQ();

	// enQ(q, (void*)111);
	// enQ(q, (void*)124);
	enQ(q, (void*)"hello1");
	enQ(q, (void*)"hello2");
	enQ(q, (void*)"hello3");
	enQ(q, (void*)"hello4");
	enQ(q, (void*)"hello5");

	// int *val = (int*)deQ((void*)q);
	// printf("%d\n",val);
	printf("size of queue = %d\n", q->size);
	while(q->size > 0){
		char *str = (char *) deQ((void *)q);
		printf("%s\n", str);
		// int *val = (int*)deQ((void*)q);
		// printf("%d\n",val);
	}
	printf("size of queue = %d\n", q->size);

	destroyQ((void*)q);

	return 0;
}

