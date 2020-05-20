#include "pid_manager.h"
#include <stdio.h>

pthread_mutex_t mutex;
int PID[MAX_PID - MIN_PID];

int allocate_map(void)
{
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < PID_LEN; i++)
    {
        PID[i] = 0;
    }
    pthread_mutex_unlock(&mutex);
    return 1;
}

int allocate_pid(void)
{
    for (int i = 0; i < PID_LEN; i++)
    {
        pthread_mutex_lock(&mutex);
        if (PID[i] == 0)
        {
            PID[i] = 1;
            pthread_mutex_unlock(&mutex);
            return i + MIN_PID;
        }
        pthread_mutex_unlock(&mutex);
    }

    return 1;
}

void release_pid(int i)
{
    if (i <= MAX_PID && i >= MIN_PID)
    {
        pthread_mutex_lock(&mutex);
        PID[i - MIN_PID] = 0;
        pthread_mutex_unlock(&mutex);
    }
    else
    {
        printf("input pid is too large pid: %d\n", i);
    }
}

void init_pid()
{
    pthread_mutex_init(&mutex, NULL);
    allocate_map();
}
