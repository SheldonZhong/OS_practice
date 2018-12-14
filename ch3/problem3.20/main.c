#include <stdio.h>
#include "pid_manager.h"

int main()
{
    int pid;
    printf("allocate_map(void): %d\n", allocate_map());

    pid = allocate_pid();
    printf("allocate_pid(void): %d\n", pid);

    release_pid(pid);
    printf("release_pid(%d):\n", pid);
}
