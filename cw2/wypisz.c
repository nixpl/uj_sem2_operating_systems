#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{	   
    printf("Proces potomny: ");
    printf("UID: %d ", getuid());
    printf("GID: %d ", getgid());
    printf("PID: %d ", getpid());
    printf("PPID: %d ", getppid());
    printf("PGID: %d\n", getpgrp());

    return 0;
}