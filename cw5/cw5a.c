#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

#define FIFO_NAME_LEN 200

#define ARGC_ERROR 1
#define MKFIFO_ERROR 2
#define SIGNAL_ERROR 3
#define EXECLP_ERROR 4
#define WAITPID_ERROR 5
#define ATEXIT_ERROR 6

char fifo_name[FIFO_NAME_LEN];

void to_runname(char name[], char runname[]) // funkcja zmieniajaca "nazwa" na "./nazwa"
{
    runname[0]='.';
    runname[1]='/';
	int i = 0;
	while (name[i] != '\0')
	{
		runname[i + 2] = name[i];
		i++;
	}
	runname[i + 2] = '\0';
    return;
}

void delete_fifo()
{
    unlink(fifo_name);
    return;
}

pid_t run_child(char program_name[], char argv[])
{
    char runname[strlen(program_name)+2];
    to_runname(program_name, runname);
    pid_t pid = fork();
    switch(pid)
    {
        case -1:
            perror("fork error");
            exit(EXIT_FAILURE);
        case 0:
            if(execlp(runname, program_name, argv, fifo_name, NULL) == -1)
            {
                perror("execlp error");
                exit(EXECLP_ERROR);
            }
        default:
            if(signal(SIGINT, delete_fifo) == SIG_ERR)
            {
                perror("signal error");
                exit(SIGNAL_ERROR);
            }
    }
    return pid;
}

int main(int argc, char *argv[])
{
    if(argc!=4)
    {
        printf("Podano %d argumentow zamiast 3\n", argc-1);
        exit(ARGC_ERROR);
    }

    strcpy(fifo_name, argv[1]);

    if(mkfifo(fifo_name, 0644) == -1)
    {
        perror("mkfifo error");
        exit(MKFIFO_ERROR);
    }

    if(signal(SIGINT, delete_fifo) == SIG_ERR)
    {
        perror("signal error\n");
        exit(SIGNAL_ERROR);
    }

    pid_t p = run_child("producent.x", argv[2]);
    pid_t k = run_child("konsument.x", argv[3]);

    if(waitpid(p, NULL, 0) == -1)
    {
        perror("waitpid error");
        exit(WAITPID_ERROR);
    }

    if(waitpid(k, NULL, 0) == -1)
    {
        perror("waitpid error");
        exit(WAITPID_ERROR);
    }

    if(atexit(delete_fifo) != 0)
    {
        perror("atexit error");
        exit(ATEXIT_ERROR);
    }
    
    return 0;
}