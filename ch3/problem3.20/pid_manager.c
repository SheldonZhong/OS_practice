#include "pid_manager.h"
#include <stdio.h>

int allocate_map(void)
{
    for (int i = 0; i < PID_LEN; i++)
    {
        PID[i] = 0;
    }

    return 1;
}

int allocate_pid(void)
{
    for (int i = 0; i < PID_LEN; i++)
    {
        if (PID[i] == 0)
        {
            PID[i] = 1;
            return i + MIN_PID;
        }
    }

    return 1;
}

void release_pid(int i)
{
    if (i <= MAX_PID && i >= MIN_PID)
    {
        PID[i - MIN_PID] = 0;
    }
    else
    {
        printf("input pid is too large pid: %d\n", i);
    }
}