#include <pthread.h>

#define MAX_RESOURCES 5

// available_resources would be involved in the race condition
int available_resources = MAX_RESOURCES;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/*
decrease available_resources by count resources
it blocks when resource is not available
and increase_count will signal to wakup decrease
when availabe_resouces changes.
*/
int decrease_count(int count)
{
    int ret = 0;

    pthread_mutex_lock(&mutex);
    while (available_resources < count) {
        pthread_cond_wait(&cond, &mutex);
    }
    available_resources -= count;
    pthread_mutex_unlock(&mutex);

    return 0;
}

// increase available_resources by count
int increase_count(int count)
{
    pthread_mutex_lock(&mutex);
    available_resources += count;
    // signal for one receiver
    // signal has better performance
    // pthread_cond_signal(&cond);
    // broadcast for multiple receiver
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

    return 0;
}

int main(void)
{
    // get resource
    int count = 1;
    decrease_count(count);
    
    // release resource
    count = 1;
    increase_count(count);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}
