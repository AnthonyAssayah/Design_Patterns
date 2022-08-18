/************************* TESTING GUARD *****************************/


#include <iostream>
#include <pthread.h>
#include "guard.cpp"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int counter = 0;

/**
 * @brief This function increments the counter over the loop - help to check if for each thread the
 * incrementation works separetely.
 * 
 * @param thread 
 * @return void* 
 */
void* foo( void *thread )
{
    Guard my_guard(lock);
    for (size_t i = 0; i < 5000; i++)
    {
      counter+=1;
    }
    
     return NULL;
     
}


int main() {

    // Test guard's function with three different threads
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    pthread_create( &thread1, NULL, foo, NULL);
    pthread_create( &thread2, NULL, foo, NULL);
    pthread_create( &thread3, NULL, foo, NULL);

    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL);
    pthread_join( thread3, NULL); 

    
    printf("counter = %d\n", counter);
    if (counter == 15000) {
        printf("SUCCESS!\n");
    }
    else {
        printf("FAILED!\n");
    }

    return 0;

}
