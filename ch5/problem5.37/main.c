#include <semaphore.h>

#define MAX_RESOURCES 5

// available_resources would be involved in the race condition
int available_resources = MAX_RESOURCES;

sem_t sem;

/*
decrease available_resources by count resources
return 0 if sufficient resources available,
otherwise return -1
*/
int decrease_count(int count)
{
    int ret = 0;

    sem_wait(&sem);
    if (available_resources < count) {
        ret = -1;
    } else {
        available_resources -= count;
        ret = 0;
    }
    sem_post(&sem);
    return ret;
}

// increase available_resources by count
int increase_count(int count)
{
    sem_wait(&sem);
    available_resources += count;
    sem_post(&sem);

    return 0;
}

int main(void)
{
    sem_init(&sem, 0, 1);

    // get resource
    int count = 1;
    while (decrease_count(count) == -1);
    
    // release resource
    count = 1;
    increase_count(count);

    sem_destroy(&sem);
}
