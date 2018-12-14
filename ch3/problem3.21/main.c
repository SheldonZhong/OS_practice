#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

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
        while (1)
        {
            if (i == 1)
            {
                printf("%d\n", i);
                break;
            }
            else
            {
                printf("%d, ", i);
            }
            i = collatz(i);
        }
    }
    else if (pid > 0)
    {
        wait(NULL);
        printf("PARENT: child completed\n");
    }
    return 0;
}
