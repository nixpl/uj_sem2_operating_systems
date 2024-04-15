#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define max_runname_size 500

#define wrong_arg_num 1
#define syg_error 2
#define wrong_method_num 3
#define fork_error 4
#define exec_error 5
#define wait_error 6
#define child_not_exist 7
#define kill_error 8

void to_runname(char name[], char runname[]) // funkcja zmieniajaca "nazwa" na "./nazwa"
{
    int i = 0;
    while (name[i] != '\0')
    {
        runname[i + 2] = name[i];
        i++;
    }
    runname[i + 2] = '\0';
}

void my_wait()
{
    int status;
    int pid = wait(&status);
    if (pid == -1)
    {
        perror("wait error");
        exit(wait_error);
    }
    for (int i = 0; i < 3; i++)
    {
        if (WIFEXITED(status))
            printf("Proces: %d zakonczyl sie ze statusem: %d\n", pid, WEXITSTATUS(status));
        if (WIFSIGNALED(status))
            printf("Proces: %d zostal zakonczony sygnalem: %d\n", pid, WTERMSIG(status));
    }
    return;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("!!!Zla ilosc argumentow, podano %d zamiast 3 arg\n", argc - 1);
        exit(wrong_arg_num);
    }
    for (int i = 0; i < 3; i++)
    {
        char runname[max_runname_size] = "./";
        pid_t pid = fork();
        switch (pid)
        {
        case -1:
            perror("fork error");
            exit(fork_error);

        case 0: // proces potomny
            to_runname(argv[1], runname);
            execlp(runname, argv[1], argv[2], argv[3], NULL);
            perror("exec error");
            exit(exec_error);
            break;
        }
    }
    sleep(1);
    if (signal(atoi(argv[3]), SIG_IGN) == SIG_ERR)
    {
        perror("signal error");
        exit(syg_error);
    }
    my_wait();

    return 0;
}