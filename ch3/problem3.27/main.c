#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[])
{
    if (argc > 3)
    {
        fprintf(stderr, "Too many arguments\n");
        return 1;
    }
    if (argc < 3)
    {
        fprintf(stderr, "Too few arguments\n");
        return 1;
    }

    char msg[BUFFER_SIZE];
    int fd[2];
    pid_t pid;
    FILE *fp;
    char *p;
    char ch;

    if (pipe(fd) == -1)
    {
        fprintf(stderr, "Pipe filed\n");
        return 1;
    }

    pid = fork();

    if (pid == 0)
    {
        // child reads from the pipe and write to file
        close(fd[WRITE_END]);
        read(fd[READ_END], msg, BUFFER_SIZE);
        fp = fopen(argv[2], "w");
        if (fp == NULL)
        {
            fprintf(stderr, "Open file %s error\n", argv[2]);
        }
        p = &msg[0];
        // printf("%s\n", msg);
        while (*p != EOF)
        {
            fprintf(fp, "%c", *p);
            p++;
        }
        // does not need to append EOF
        // seems like library handles it
        // fprintf(fp, "%c", EOF);

        close(fd[READ_END]);
    }
    else
    {
        close(fd[READ_END]);
        fp = fopen(argv[1], "r");
        if (fp == NULL)
        {
            fprintf(stderr, "Open file %s error\n", argv[1]);
            return 1;
        }
        p = &msg[0];
        while ((ch = fgetc(fp)) != EOF)
        {
            sprintf(p, "%c", ch);
            p++;
        }
        // should append one EOF to stop child from loop
        sprintf(p, "%c", EOF);
        // printf("%s", msg);
        write(fd[WRITE_END], msg, strlen(msg) + 1);
        close(fd[WRITE_END]);
    }

    // parent reads from the source file and sends to pipe
    return 0;
}