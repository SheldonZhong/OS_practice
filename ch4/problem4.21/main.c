#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>

#define MAX_LENGTH 100

struct prog_param
{
    int *arr;
    int end;
};

int avg = 0;
int min = 0;
int max = 0;

void *maxFunc(void *param)
{
    struct prog_param *s = param;
    int end = s->end;
    int maxl = INT_MIN;
    for (int i = 0; i < end; i++)
    {
        if (maxl < s->arr[i])
        {
            maxl = s->arr[i];
        }
    }

    max = maxl;
    pthread_exit(0);
}

void *minFunc(void *param)
{
    struct prog_param *s = param;
    int end = s->end;
    int minl = INT_MAX;

    for (int i = 0; i < end; i++)
    {
        if (minl > s->arr[i])
        {
            minl = s->arr[i];
        }
    }

    min = minl;
    pthread_exit(0);
}

void *avgFunc(void *param)
{
    struct prog_param *s = param;
    int end = s->end;

    int sum = 0;
    for (int i = 0; i < end; i++)
    {
        sum += s->arr[i];
    }

    avg = sum / end;
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    char *arr;
    int buffer[MAX_LENGTH];
    pthread_t tid_avg;
    pthread_t tid_min;
    pthread_t tid_max;

    pthread_attr_t attr;

    int temp = 0;
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            sscanf(argv[i], "%d", &temp);
            buffer[i - 1] = temp;
        }
    }
    else
    {
        return 1;
    }
    struct prog_param param = {&buffer[0], argc - 1};

    // get thread attributes
    pthread_attr_init(&attr);

    // create a thread
    pthread_create(&tid_avg, &attr, avgFunc, &param);
    pthread_create(&tid_min, &attr, minFunc, &param);
    pthread_create(&tid_max, &attr, maxFunc, &param);

    // wait for each thread to finish
    pthread_join(tid_avg, NULL);
    pthread_join(tid_min, NULL);
    pthread_join(tid_max, NULL);

    printf("The average value is %d\n", avg);
    printf("The minimum value is %d\n", min);
    printf("The maximum value is %d\n", max);
}
