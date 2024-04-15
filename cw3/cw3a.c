#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define wrong_arg_num 1
#define syg_error 2
#define wrong_method_num 3

void handler(int syg)
{
	printf("Przechwycono sygnal: %s o id: %d\n", strsignal(syg), syg);
	return;
}

int main(int argc, char *argv[])
{
	printf("PID tego procesu to: %d\n", getpid());

	if (argc != 3)
	{
		printf("Zla ilosc argumentow, podano %d zamiast 2 arg\n", argc - 1);
		exit(wrong_arg_num);
	}

	int syg = atoi(argv[2]);

	if (syg != 9 && syg != 19)
	{
		switch (atoi(argv[1]))
		{
		case 1: // opcja domyslna
			if (signal(syg, SIG_DFL) == SIG_ERR)
			{
				perror("signal error");
				exit(syg_error);
			}
			break;

		case 2: // ignorowanie sygnalu
			if (signal(syg, SIG_IGN) == SIG_ERR)
			{
				perror("signal error");
				exit(syg_error);
			}
			break;

		case 3: // przechwycenie sygnalu
			if (signal(syg, handler) == SIG_ERR)
			{
				perror("signal error");
				exit(syg_error);
			}
			break;

		default:
			printf("Opcja o nr: %d nie istnieje\n", atoi(argv[1]));
			exit(wrong_method_num);
		}
	}
	else
	{
		printf("Sygnaly o id 9 i 19 nie sa obslugiwane przez funkcje signal()\n");
		exit(syg_error);
	}

	printf("Oczekiwanie na sygnal...\n");
	pause();
	return 0;
}
