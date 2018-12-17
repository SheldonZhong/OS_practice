#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define HIST_SIZE 10

int main(void)
{
    char *args[MAX_LINE / 2 + 1];
    // maximun possible arguments:
    // command + (space(1 char) + argument(1 char)) * 40
    char line[MAX_LINE];
    // command having maximun length
    char *hist[HIST_SIZE];
    int histc = 0;

    int argc = 0;
    int should_run = 1;
    int start = -1;
    int len;
    int background = 0;
    int command = 0;
    pid_t pid;
    while (should_run)
    {
        printf("osh>");
        fflush(stdout);

        fgets(&line[0], MAX_LINE, stdin);
        // remove \n
        line[strlen(&line[0]) - 1] = '\0';

        // handle history early
        if (strcmp("history", &line[0]) == 0)
        {
            for (int i = histc - 1; i >= 0; i--)
            {
                printf("\t%d %s\n", i + 1, hist[i]);
            }
            // printf("HISTORY\n");
            continue;
        }
        len = strlen(&line[0]);
        if (line[0] == '!' && (len > 1))
        {
            // printf("Special handle\n");
            if (line[1] == '!')
            {
                strcpy(&line[0], hist[histc - 1]);
            }
            else if ((line[1] > 47) && (line[1] <= 58))
            {
                // only support 20 history command regardless HIST_SIZE
                if (len == 2)
                {
                    command = line[1] - 48;
                }
                else
                {
                    command = 10 + line[2] - 48;
                }

                if ((command <= histc) && (command > 0))
                {
                    strcpy(&line[0], hist[command - 1]);
                }
            }
        }
        // printf("Gets line: %s\n", &line[0]);
        // save history
        if (histc < (HIST_SIZE))
        {
            // printf("Saving history %d %s\n", histc, &line[0]);
            strcpy(hist[histc], &line[0]);
            // printf("Saved history %s\n", hist[histc]);
            histc++;
            hist[histc] = malloc(sizeof(char *));
        }
        else
        {
            for (int i = 0; i < (HIST_SIZE - 1); i++)
            {
                // printf("Moving %d to %d\n", i + 1, i);
                hist[i] = hist[i + 1];
            }
            strcpy(hist[histc], &line[0]);
        }
        // printf("strlen(%d)\n", len);
        argc = 0;
        start = -1;
        background = 0;

        // parse command
        for (int i = 0; i < len; i++)
        {
            if (start < 0)
            {
                // start of a string, multiple space are allowed
                if (line[i] != '\0')
                {
                    start = i;
                    args[argc] = &line[start];
                }
            }

            // argument seperator
            if ((line[i] == ' ') && (line[i - 1] != '\0'))
            {
                line[i] = '\0';
                argc++;
                start = -1;
            }

            // handle newline trailing space
            if (i == (len - 1))
            {
                // line[i] = '\0';
                argc++;
                start = -1;
                if (line[i - 1] == '&')
                {
                    background = 1;
                }
            }
        }
        // re = fscanf(stdin, "%s", args[0]);
        // for (int i = 0; i < argc; i++)
        // {
        //     printf("%s\n", args[i]);
        // }

        pid = fork();

        if (pid == 0)
        {
            execvp(args[0], args);
            return 0;
        }
        else if (pid > 0)
        {
            if (background)
            {
                wait(NULL);
            }
            printf("PARENT: child finishes\n");
        }
    }

    return 0;
}
