/*
Modified version of problem 3.21
parent process prints the result
using POSIX shared memory described in Section 3.5.1
*/
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int collatz(int n)
{
    if ((n % 2) == 0)
    {
        return n / 2;
    }
    else
    {
        return 3 * n + 1;
    }
}

int main(int argc, char *argv[])
{
    int i;
    pid_t pid;
    const int SIZE = 4096;
    const char *name = "SharedMem";

    int shm_fd;
    void *ptr;
    if (argc > 1)
    {
        sscanf(argv[1], "%d", &i);
        if (i < 1)
        {
            printf("Error input: %d\n", i);
            return 0;
        }
    }
    else
    {
        return 1;
    }

    pid = fork();

    if (pid == 0)
    {
        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, SIZE);
        ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
        printf("CHILD:\n");
        while (1)
        {
            if (i == 1)
            {
                sprintf(ptr, "%d\n", i);
                printf("%d\n", i);
                ptr += strlen(ptr);
                break;
            }
            else
            {
                sprintf(ptr, "%d, ", i);
                printf("%d, ", i);
                ptr += strlen(ptr);
            }
            i = collatz(i);
        }
    }
    else if (pid > 0)
    {
        wait(NULL);
        printf("PARENT: child completed\n");

        shm_fd = shm_open(name, O_RDONLY, 0666);
        ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
        printf("Parent printing:\n");

        printf("%s", (char *)ptr);
        shm_unlink(name);
    }
    return 0;
}
