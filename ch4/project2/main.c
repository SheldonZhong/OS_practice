#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>

int arr[100];

typedef struct
{
    int start;
    int end;
} Parameters;

void *sort(void *param)
{
    Parameters *params = param;
    int start = params->start;
    int end = params->end;
    int tmp = 0;

    // bubble sort
    for (int i = start; i < end; i++)
    {
        for (int j = i; j < end; j++)
        {
            if (arr[i] > arr[j])
            {
                tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }
    }
    pthread_exit(0);
}

void *merge(void *param)
{
    Parameters *params = param;
    int split = params->start;
    int end = params->end;
    int temp[end];
    int idxa = 0;
    int idxb = split;

    for (int i = 0; i < end; i++)
    {
        if ((idxa < split) && (idxb < end))
        {
            if (arr[idxa] < arr[idxb])
            {
                temp[i] = arr[idxa++];
            }
            else
            {
                temp[i] = arr[idxb++];
            }
        }
        else
        {
            // reach the end, append all
            if (idxa == split)
            {
                temp[i] = arr[idxb++];
            }
            else{
                temp[i] = arr[idxa++];
            }
        }
    }

    for (int i = 0; i < end; i++)
    {
        arr[i] = temp[i];
    }

    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    pthread_t tid_sort1, tid_sort2, tid_merge;
    pthread_attr_t attr;
    Parameters *param_sort1, *param_sort2;
    param_sort1 = malloc(sizeof(Parameters));
    param_sort2 = malloc(sizeof(Parameters));
    int split = (argc - 1) / 2;

    param_sort1->start = 0;
    param_sort1->end = split;

    param_sort2->start = split;
    param_sort2->end = argc - 1;

    for (int i = 1; i < argc; i++)
    {
        arr[i - 1] = atoi(argv[i]);
    }

    pthread_attr_init(&attr);

    pthread_create(&tid_sort2, &attr, sort, param_sort2);
    pthread_create(&tid_sort1, &attr, sort, param_sort1);

    pthread_join(tid_sort1, NULL);
    pthread_join(tid_sort2, NULL);

    pthread_create(&tid_merge, &attr, merge, param_sort2);
    pthread_join(tid_merge, NULL);

    for (int i = 0; i < argc - 1; i++)
    {
        printf("%d ", arr[i]);
    }

    printf("\n");
}
