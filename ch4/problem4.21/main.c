#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

#define MAX_LENGTH 100

int maxFunc(int arr[MAX_LENGTH], int end)
{
    int max = INT_MIN;
    for (int i = 0; i < end; i++)
    {
        if (max < arr[i])
        {
            max = arr[i];
        }
    }

    return max;
}

int minFunc(int arr[MAX_LENGTH], int end)
{
    int min = INT_MAX;
    for (int i = 0; i < end; i++)
    {
        if (min > arr[i])
        {
            min = arr[i];
        }
    }

    return min;
}

int avgFunc(int arr[MAX_LENGTH], int end)
{
    int sum = 0;
    for (int i = 0; i < end; i++)
    {
        sum += arr[i];
    }

    return sum / end;
}

int main(int argc, char *argv[])
{
    char *arr;
    int buffer[MAX_LENGTH];
    pid_t pid;
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

    int avg = 0;
    int min = 0;
    int max = 0;

    avg = avgFunc(buffer, argc - 1);
    min = minFunc(buffer, argc - 1);
    max = maxFunc(buffer, argc - 1);

    printf("The average value is %d\n", avg);
    printf("The minimum value is %d\n", min);
    printf("The maximum value is %d\n", max);
}
