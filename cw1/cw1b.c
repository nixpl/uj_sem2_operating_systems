#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	printf("Proces macierzysty: ");
	printf("UID: %d ", getuid());
    	printf("GID: %d ", getgid());
    	printf("PID: %d ", getpid());
    	printf("PPID: %d ", getppid());
    	printf("PGID: %d\n", getpgrp());

	for(int i=0; i<3; i++)
	{
		switch(fork())
		{
			case -1:
			perror("fork error");
			exit(1);

			case 0:
			printf("Proces potomny: ");
                	printf("UID: %d ", getuid());
    			printf("GID: %d ", getgid());
    			printf("PID: %d ", getpid());
    			printf("PPID: %d ", getppid());
    			printf("PGID: %d\n", getpgrp());
			break;

			default:
			wait(NULL);
		}
	}

	return 0;
}
