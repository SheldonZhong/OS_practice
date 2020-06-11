#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>

int hits = 0;
pthread_mutex_t mutex;

double frand()
{
    return (double)rand() / (double)RAND_MAX;
}

void *cal_pi(void *param)
{
    int total = atoi(param);
    double x, y;
    for (int i = 0; i < total; i++)
    {
        x = frand();
        y = frand();

        if ((x * x + y * y) < 1)
        {
            pthread_mutex_lock(&mutex);
            hits++;
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("%s [num_sim / thread] [num_thread]\n", argv[0]);
        return 1;
    }
    srand(time(NULL));
    
    int nproc = atoi(argv[2]);
    pthread_t* tid = calloc(nproc, sizeof(*tid));
    pthread_attr_t attr;

    // get thread attributes
    pthread_attr_init(&attr);

    // create a thread
    for (int i = 0; i < nproc; i++) {
        pthread_create(tid + i, &attr, cal_pi, argv[1]);
    }

    // wait for each thread to finish
    for (int i = 0; i < nproc; i++) {
        pthread_join(tid[i], NULL);
    }
    int total = atoi(argv[1]) * atoi(argv[2]);

    printf("Monte Carlo Pi: %f\n", 4 * (double)hits / (double)total);
}
