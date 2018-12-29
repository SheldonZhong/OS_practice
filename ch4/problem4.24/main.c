#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096
// Maximum input: 4096 * 32 = 131072
u_int32_t buffer[BUFFER_SIZE];
const int op = 1;

void *primes(void *param)
{
    int *p = param;
    int n = *p;
    int shift, idx, tmp, prime;
    for (int i = 2; i * i <= n; i++)
    {
        idx = i / 32;
        shift = i % 32;
        prime = (!(buffer[idx] & (op << shift)));
        for (int j = 2; prime && (tmp <= n); j++)
        {
            // multiple of primes
            tmp = i * j;
            shift = tmp % 32;
            idx = tmp / 32;
            buffer[idx] |= (op << shift);
        }
        tmp = 0;
    }

    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    pthread_attr_t attr;
    int n, idx, shift;
    scanf("%d", &n);

    // get thread attributes
    pthread_attr_init(&attr);

    // create a thread
    pthread_create(&tid, &attr, primes, &n);

    // wait for each thread to finish
    pthread_join(tid, NULL);

    for (int i = 2; i <= n; i++)
    {
        idx = i / 32;
        shift = i % 32;
        if (!(buffer[idx] & (op << shift)))
        {
            printf("%d ", i);
        }
    }
    printf("\n");
}
