#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

u_int32_t buffer[BUFFER_SIZE];

void *primes(void *param)
{
    int *p = param;
    int n = *p;

    buffer[0] = 0;
    buffer[1] = 1;
    for (int i = 2; i < n; i++)
    {
        buffer[i] = buffer[i - 1] + buffer[i - 2];
    }

    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    pthread_attr_t attr;
    int n;
    scanf("%d", &n);
    if (n >= 48)
    {
        printf("input %d would be overflow, the overflowed result is shown anyway\n", n);
    }
    // get thread attributes
    pthread_attr_init(&attr);

    // create a thread
    pthread_create(&tid, &attr, primes, &n);

    // wait for each thread to finish
    pthread_join(tid, NULL);

    for (int i = 0; i < n; i++)
    {
        printf("%d ", buffer[i]);
    }
    printf("\n");
}
