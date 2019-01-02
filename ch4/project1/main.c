#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>

// #define ROW 0
// #define COLUMN 1
// #define DIAGONAL1 2
// #define DIAGONAL2 3
// #define GRID 4

typedef enum
{
    ROW,
    COLUMN,
    DIAGONAL1,
    DIAGONAL2,
    GRID
} Valid_Type;

char sudoku[9][9];
// char row[9];
int mask = 0x03FE;
int results[29];

typedef struct
{
    int row;
    int column;
    Valid_Type type;
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
        printf("ROW\n");
        for (char *p = &sudoku[params->column][0]; p != &sudoku[params->column][9]; p++)
        {
            result |= (1 << *p);
            printf("%d ", *p);
        }
        printf("\n");
        results[params->column] = result;
        break;

    case COLUMN:
        printf("COLUMN\n");
        for (int i = 0; i < 9; i++)
        {
            row = &sudoku[i][0];
            cell = row[params->row];
            result |= (1 << cell);
            printf("%d ", cell);
        }
        printf("\n");
        results[params->row + 9] = result;
        break;

    case DIAGONAL1:
        printf("DIAGONAL1\n");
        for (int i = 0; i < 9; i++)
        {
            row = &sudoku[i][0];
            cell = row[i];
            result |= (1 << cell);
            printf("%d ", cell);
        }
        printf("\n");
        results[27] = result;
        break;

    case DIAGONAL2:
        printf("DIAGONAL2\n");
        for (int i = 0; i < 9; i++)
        {
            row = &sudoku[i][0];
            cell = row[8 - i];
            result |= (1 << cell);
            printf("%d ", cell);
        }
        printf("\n");
        results[28] = result;
        break;

    case GRID:
        printf("GRID\n");
        row = &sudoku[params->column][0];
        for (int i = 0; i < 9; i++)
        {
            row = &sudoku[params->column + i / 3][0];
            cell = row[params->row + i % 3];
            result |= (1 << cell);
            printf("%d ", cell);
        }
        printf("\n");
        // (0, 0) -> 0
        // (0, 1) -> 1
        // (0, 2) -> 2
        // (1, 0) -> 3
        // (1, 1) -> 4
        // (1, 2) -> 5
        // (2, 0) -> 6
        // (2, 1) -> 7
        // (2, 2) -> 8
        results[((params->column / 3) * 3) + params->row / 3 + 18] = result;
        break;

    default:
        break;
    }

    printf("True %d\n", result);

    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    pthread_t tid[29];
    pthread_attr_t attr;
    int n;
    Parameters *params[29];
    Parameters *param;

    // Parameters *params = malloc(sizeof(Parameters));
    for (int i = 0; i < 9; i++)
    {
        param = malloc(sizeof(Parameters));
        param->type = COLUMN;
        param->column = 0;
        param->row = i;
        params[i] = param;
    }

    for (int i = 0; i < 9; i++)
    {
        param = malloc(sizeof(Parameters));
        param->type = ROW;
        param->column = i;
        param->row = 0;
        params[i + 9] = param;
    }

    for (int i = 0; i < 9; i += 3)
    {
        for (int j = 0; j < 9; j += 3)
        {
            param = malloc(sizeof(Parameters));
            param->type = GRID;
            param->column = i;
            param->row = j;
            params[i / 3 * 3 + j / 3 + 18] = param;
        }
    }
    param = malloc(sizeof(Parameters));
    param->type = DIAGONAL1;
    params[27] = param;

    param = malloc(sizeof(Parameters));
    param->type = DIAGONAL2;
    params[28] = param;

    FILE *fp = fopen("sudoku.txt", "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error while opening file\n");
        return 1;
    }

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            fscanf(fp, "%d", &n);
            sudoku[i][j] = n;
        }
    }

    pthread_attr_init(&attr);

    // for (int i = 0; i < 29; i++)
    //     pthread_create(&tid[i], &attr, valid_worker, params[i]);

    // for (int i = 0; i < 29; i++)
    //     pthread_join(tid[i], NULL);
    for (int i = 0; i < 29; i++)
    {
        pthread_create(&tid[i], &attr, valid_worker, params[i]);
        pthread_join(tid[i], NULL);
    }

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < 29; i++)
    {
        if (results[i] != mask)
        {
            printf("Not valid\n");
            // break;
        }
    }
}
