#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>

#define CONSTRAINT_ROW_OFFSET 0
#define CONSTRAINT_COLUMN_OFFSET 9 + CONSTRAINT_ROW_OFFSET
#define CONSTRAINT_GRID_OFFSET 9 + CONSTRAINT_COLUMN_OFFSET
#define CONSTRAINT_DIAGONAL1_OFFSET CONSTRAINT_SUDOKU
#define CONSTRAINT_DIAGONAL2_OFFSET CONSTRAINT_DIAGONAL1_OFFSET + 1

#define CONSTRAINT_SUDOKU 9 + CONSTRAINT_GRID_OFFSET
#define CONSTRAINT_SUDOKU_DIAG CONSTRAINT_DIAGONAL2_OFFSET + 1

// variations would be add extra constraints on diagonal
#define SUDOKU_RULE CONSTRAINT_SUDOKU
// #define SUDOKU_RULE CONSTRAINT_SUDOKU_DIAG

typedef enum
{
    ROW,
    COLUMN,
    DIAGONAL1,
    DIAGONAL2,
    GRID
} Valid_Type;

char sudoku[9][9];
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
        for (char *p = &sudoku[params->column][0]; p != &sudoku[params->column][9]; p++)
        {
            result |= (1 << *p);
        }
        results[params->column + CONSTRAINT_ROW_OFFSET
    ] = result;
        break;

    case COLUMN:
        for (int i = 0; i < 9; i++)
        {
            row = &sudoku[i][0];
            cell = row[params->row];
            result |= (1 << cell);
        }
        results[params->row + CONSTRAINT_COLUMN_OFFSET] = result;
        break;

    case DIAGONAL1:
        for (int i = 0; i < 9; i++)
        {
            row = &sudoku[i][0];
            cell = row[i];
            result |= (1 << cell);
        }
        results[CONSTRAINT_DIAGONAL1_OFFSET] = result;
        break;

    case DIAGONAL2:
        for (int i = 0; i < 9; i++)
        {
            row = &sudoku[i][0];
            cell = row[8 - i];
            result |= (1 << cell);
        }
        results[CONSTRAINT_DIAGONAL2_OFFSET] = result;
        break;

    case GRID:
        row = &sudoku[params->column][0];
        for (int i = 0; i < 9; i++)
        {
            row = &sudoku[params->column + i / 3][0];
            cell = row[params->row + i % 3];
            result |= (1 << cell);
        }
        results[((params->column / 3) * 3) + params->row / 3 + CONSTRAINT_GRID_OFFSET] = result;
        break;

    default:
        break;
    }

    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    pthread_t tid[29];
    pthread_attr_t attr;
    int n;
    Parameters *params[29];
    Parameters *param;

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
            params[i / 3 * 3 + j / 3 + CONSTRAINT_GRID_OFFSET] = param;
        }
    }
    param = malloc(sizeof(Parameters));
    param->type = DIAGONAL1;
    params[CONSTRAINT_DIAGONAL1_OFFSET] = param;

    param = malloc(sizeof(Parameters));
    param->type = DIAGONAL2;
    params[CONSTRAINT_DIAGONAL2_OFFSET] = param;

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

    for (int i = 0; i < SUDOKU_RULE; i++)
        pthread_create(&tid[i], &attr, valid_worker, params[i]);

    for (int i = 0; i < SUDOKU_RULE; i++)
        pthread_join(tid[i], NULL);

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < SUDOKU_RULE; i++)
    {
        if (results[i] != mask)
        {
            printf("Not a valid sudoku\n");
            return 1;
        }
    }
    printf("Valid sudoku\n");
    if (SUDOKU_RULE == CONSTRAINT_SUDOKU)
    {
        printf("Sudoku without diagonal\n");
    }
    else
    {
        printf("Sudoku with diagonal check\n");
    }
}
