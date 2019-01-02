#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>

#define ROW 0
#define COLUMN 1
#define DIAGONAL1 2
#define DIAGONAL2 3
#define GRID 4

char sudoku[9][9];
// char row[9];
int mask = 0x01FF;

typedef struct
{
    int row;
    int column;
    int type;
} Parameters;

void *valid_worker(void *param)
{
    Parameters *params = param;
    int result = 0;
    char cell;
    char *row;
    
    switch (params->type)
    {
    case ROW:
        for (char *p = &sudoku[params->column][0]; p != &p[9]; p++)
        {
            result |= (1 << *p);
        }
        break;

    case COLUMN:
        for (int i = 0; i < 9; i++)
        {
            row = &sudoku[i][0];
            cell = row[params->row];
            result |= (1 << cell);
        }
        break;

    case DIAGONAL1:
        for (int i = 0; i < 9; i++)
        {
            row = &sudoku[i][0];
            cell = row[i];
            result |= (1 << cell);
        }
        break;

    case DIAGONAL2:
        for (int i = 0; i < 9; i++)
        {
            row = &sudoku[i][0];
            cell = row[8 - i];
            result |= (1 << cell);
        }
        break;

    case GRID:
        row = &sudoku[params->column][0];
        for (int i = 0; i < 9; i++)
        {
            row = &sudoku[params->column + i / 3][0];
            cell = row[params->row + i % 3];
            result |= (1 << cell);
        }
        break;

    default:
        break;
    }

    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    pthread_attr_t attr;
    int n;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            scanf("%d", &n);
            sudoku[i][j] = n;
        }
    }

    // pthread_attr_init(&attr);

    // pthread_create(&tid, &attr, valid_worker, &n);

    // pthread_join(tid, NULL);

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }
}
