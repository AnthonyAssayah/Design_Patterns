#include <pthread.h>
#include <iostream>

using namespace std;


class Guard {

    private:
        pthread_mutex_t *lock;
    public:
        Guard(pthread_mutex_t mutex) {
            this->lock = &mutex;
            pthread_mutex_lock(this->lock);
        }

        ~Guard() {
            pthread_mutex_unlock(this->lock);
        }
    
};
