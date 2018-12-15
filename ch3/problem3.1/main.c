#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

#define SIZE 5

int val = 5;

int main()
{
	pid_t pid;

	pid = fork();

	if (pid == 0)
	{
		val += 15;
		printf("CHILD: %d \n", val);
		// return 0;
	}
	else if (pid > 0)
	{
		wait(NULL);
		printf("PARENT: %d \n", val);
		// return 0;
	}
	return 0;
}
