#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
	pid_t pid, pid1;
	
	pid = fork();
	
	if (pid == 0) {
        pid1 = getpid();
        printf("child: pid = %d\n", pid);
        printf("child: pid1 = %d\n", pid1);
        // return 0;
	}
	else if (pid > 0 ) {
        pid1 = getpid();
        printf("parent: pid = %d\n", pid);
        printf("parent: pid1 = %d\n", pid1);
	wait(NULL);
	printf("PARENT: Child completed\n");
        // return 0;
	}
	return 0;
}
