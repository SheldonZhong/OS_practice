#include <semaphore.h>

#define MAX_RESOURCES 5

// available_resources would be involved in the race condition
int available_resources = MAX_RESOURCES;

/*
decrease available_resources by count resources
return 0 if sufficient resources available,
otherwise return -1
*/
int decrease_count(int count)
{
    if (available_resources < count)
        return -1;
    else
    {
        available_resources -= count;

        return 0;
    }
}

// increase available_resources by count
int increase_count(int count)
{
    available_resources += count;

    return 0;
}

int main(void)
{
    sem_t sem;

    sem_init(&sem, 0, 1);

    sem_wait(&sem);

    sem_post(&sem);
}
