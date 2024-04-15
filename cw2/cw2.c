#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void my_wait()
{
    if(wait(NULL)==-1)
        {
            perror("wait error");
            exit(3);
        }
    return;
}

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
            execlp("./wypisz.x", "wypisz.x", NULL);
            perror("exec error");
            exit(2);
            break;

            default:
            my_wait();
        }
    }

    return 0;
}
