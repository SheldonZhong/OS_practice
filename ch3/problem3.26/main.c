#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

void re_cap(char *string)
{
    for (int i = 0; i < strlen(string); i++)
    {
        if ((string[i] > 64) && (string[i] < 91))
        {
            // uppercase
            string[i] += 32;
        }
        else if (string[i] > 89 && (string[i] < 123))
        {
            // lowercase
            string[i] -= 32;
        }
    }
}

int main(void)
{
    char write_msg[BUFFER_SIZE] = "Grettings!";
    char read_msg[BUFFER_SIZE];
    int fd[2];
    int fd_cap[2];

    pid_t pid;
    if (pipe(fd) == -1)
    {
        fprintf(stderr, "Pipe filed");
        return 1;
    }
    if (pipe(fd_cap) == -1)
    {
        fprintf(stderr, "Pipe filed");
        return 1;
    }

    pid = fork();

    // Parent sends to child, child changes to capital
    // and child sends back to parent using cap_pipe
    if (pid == 0)
    {
        close(fd[WRITE_END]);

        read(fd[READ_END], read_msg, BUFFER_SIZE);
        printf("child reads: %s\n", read_msg);
        re_cap(&read_msg[0]);
        printf("child reverses: %s\n", read_msg);

        close(fd[READ_END]);

        close(fd_cap[READ_END]);
        write(fd_cap[WRITE_END], read_msg, BUFFER_SIZE);
        close(fd_cap[WRITE_END]);
    }
    else
    {
        // sends to child
        close(fd[READ_END]);
        write(fd[WRITE_END], write_msg, strlen(write_msg) + 1);
        close(fd[WRITE_END]);

        // reads from child
        close(fd_cap[WRITE_END]);
        read(fd_cap[READ_END], read_msg, BUFFER_SIZE);
        printf("Parent receives %s\n", read_msg);
        close(fd_cap[READ_END]);
    }

    return 0;
}