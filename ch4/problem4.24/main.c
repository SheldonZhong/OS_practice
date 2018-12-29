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
int cnt = 0;
void *primes(void *param)
{
    int *p = param;
    int n = *p;
    int shift, idx, tmp, prime;
    // both factors should less than or equal to sqrt(n)
    for (int i = 2; i * i <= n; i++)
    {
        idx = i / 32;
        shift = i % 32;
        prime = (!(buffer[idx] & (op << shift)));
        // ternaory operator to skip prime increment factor of 2
        // actually we could skip all non-prime factors, but we lost the information
        for (int j = i * i; prime && (j <= n); j += ((i > 2 ? 2 : 1) * i))
        {
            // i is prime, i * i, i * (i + 1), ...
            // because 1 * i, 2 * i, ...., (i - 1) * i, have been calculated
            // 4, 6, 8, ....
            // 9, 12(skipped), 15, 18(skipped), ...
            // 25, 30(skipped), 35, 40(skipped), 45(should skipped for 3), 50(skipped), ...
            // multiple of primes
            shift = j % 32;
            idx = j / 32;
            buffer[idx] |= (op << shift);
            cnt++;
        }
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
