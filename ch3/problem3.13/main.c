#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	
	pid = fork();
	
	if (pid == 0) {
        printf("CHILD: line I\n");
        execlp("/bin/ls", "pwd", NULL);
        printf("CHILD: line J");
        // return 0;
	}
	else if (pid > 0 ) {
		wait(NULL);
		printf("PARENT: Child completed\n");
        // return 0;
	}
	return 0;
}
