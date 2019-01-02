#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX_LENGTH 100

int avg, end;
int max = INT_MIN;
int min = INT_MAX;
int *arr;

void *maxFunc(void *param)
{
    for (int i = 0; i < end; i++)
    {
        if (max < arr[i])
        {
            max = arr[i];
        }
    }

    pthread_exit(0);
}

void *minFunc(void *param)
{
    for (int i = 0; i < end; i++)
    {
        if (min > arr[i])
        {
            min = arr[i];
        }
    }
    
    pthread_exit(0);
}

void *avgFunc(void *param)
{
    int sum = 0;
    for (int i = 0; i < end; i++)
    {
        sum += arr[i];
    }

    avg = sum / end;
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    int buffer[MAX_LENGTH];
    pthread_t tid_avg;
    pthread_t tid_min;
    pthread_t tid_max;

    pthread_attr_t attr;

    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            buffer[i - 1] = atoi(argv[i]);
        }
    }
    else
    {
        return 1;
    }

    arr = &buffer[0];
    end = argc - 1;

    // get thread attributes
    pthread_attr_init(&attr);

    // create threads
    pthread_create(&tid_avg, &attr, avgFunc, NULL);
    pthread_create(&tid_min, &attr, minFunc, NULL);
    pthread_create(&tid_max, &attr, maxFunc, NULL);

    // wait for each thread to finish
    pthread_join(tid_avg, NULL);
    pthread_join(tid_min, NULL);
    pthread_join(tid_max, NULL);

    printf("The average value is %d\n", avg);
    printf("The minimum value is %d\n", min);
    printf("The maximum value is %d\n", max);
}
