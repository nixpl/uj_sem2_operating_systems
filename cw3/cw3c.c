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
#define setpgid_error 9

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

void my_wait(pid_t pid)
{
	int status;
	if (wait(&status) == -1)
	{
		perror("wait error");
		exit(wait_error);
	}
	int syg = WTERMSIG(status);

	if (WIFSIGNALED(status))
		printf("Proces potomny: %d zostal zakonczony sygnalem: %s o numerze: %d\n", pid, strsignal(syg), syg);
	return;

	return;
}

int main(int argc, char *argv[])
{
	if (argc != 5)
	{
		printf("Zla ilosc argumentow, podano %d zamiast 4 arg\n", argc - 1);
		exit(wrong_arg_num);
	}
	pid_t pid = fork();

	switch (pid)
	{
	case -1:
		perror("fork error");
		exit(fork_error);

	case 0: // wywolanie prgramu od sygnalow
		if (setpgid(0, 0) == -1)
		{
			perror("setpgid error");
			exit(setpgid_error);
		}

		char runname[max_runname_size] = "./";
		to_runname(argv[1], runname);
		execlp(runname, argv[1], argv[2], argv[3], argv[4], NULL);
		perror("exec error");
		exit(exec_error);
		break;

	default:
		sleep(1);

		if (kill(pid, 0) == -1) // sprawdzenie czy istnieje program do ktorego wysylamy sygnal
		{
			perror("kill error");
			exit(child_not_exist);
		}

		if (kill(pid * (-1), atoi(argv[4])) == -1) // wyslanie sygnalu
		{
			perror("kill error");
			exit(kill_error);
		}

		my_wait(pid);
	}

	return 0;
}
