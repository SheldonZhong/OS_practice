#define _GNU_SOURCE
#include <stdio.h>
#include "pid_manager.h"
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

int threadFunc()
{
    int r = rand();
    int pid = allocate_pid();
    printf("thread %d starting to sleep for %d ms\n", pid, r / 1000);
    // sleep for a random of time
    usleep(r / 1000);
    release_pid(pid);
    printf("thread %d releases\n", pid);
    return 0;
}

#define STACK_SIZE 1024 * 1024

int main()
{
    int r;
    char *stack;
    char *stackTop;
    pid_t pid;
    init_pid();
    
    srand(time(NULL));
    printf("time %ld\n", time(NULL));
    stack = malloc(STACK_SIZE);
    if (stack == NULL)
    {
        fprintf(stderr, "malloc error\n");
    }
    stackTop = stack + STACK_SIZE;
    for (int i = 0; i < 100; i++)
    {
        stack = malloc(STACK_SIZE);
        if (stack == NULL)
        {
            fprintf(stderr, "malloc error\n");
        }
        stackTop = stack + STACK_SIZE;
        fflush(stdout);
        pid = clone(threadFunc, stackTop, CLONE_VM, 0);
        if (pid == -1)
        {
            fprintf(stderr, "clone error\n");
        }
    }
    sleep(5);
}
