#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>

#include "semaphore_fun.h"

#define WRONG_ARG_NUM 1
#define FOPEN_ERROR 2
#define FPRINTF_ERROR 3
#define FCLOSE_ERROR 4
#define FORK_ERROR 5
#define EXECLP_ERROR 6
#define SIGNAL_ERROR 7
#define WAITPID_ERROR 8
#define FSCANF_ERROR 9

#define FILE_CONTENT_SIZE 20

const char* semaphore_name = "semafor";

void del_sem() 
{
    delete_semaphore(semaphore_name);
}

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

void my_fclose(FILE* file)
{
    if (fclose(file) != 0) 
    {
        perror("fclose error");
        exit(FCLOSE_ERROR);
    }
}

int main(int argc, char *argv[]) 
{
    if (argc != 4) 
    {
        printf("Zla ilosc argumentow (%d), spodziewano sie 3\n", argc);
        exit(WRONG_ARG_NUM);
    }

    char* program_name = argv[1];
    char run_program_name[strlen(program_name)+2];
    to_runname(program_name, run_program_name);

    int num_processes = atoi(argv[2]);

    char* num_sections = argv[3];

    sem_t* semaphore = create_semaphore(semaphore_name, O_EXCL | O_CREAT, 0666, 1);
    printf("Stworzono semafor o adresie: %p\n", (void *)semaphore);
    printf("Początkowa wartość semafora wynosi: 1\n");

    FILE* file = fopen("numer.txt", "w");
    if (file == NULL) 
    {
        perror("fopen error");
        exit(FOPEN_ERROR);
    }
    if (fprintf(file, "0") < 0) 
    {
        perror("fprintf error");
        my_fclose(file);
        exit(FPRINTF_ERROR);
    }
    my_fclose(file);

    for (int i = 0; i < num_processes; i++) 
    {
        switch (fork()) 
        {
            case -1: //error
                perror("fork error\n");
                exit(FORK_ERROR);
            case 0: //potomny
                execlp(run_program_name, program_name, num_sections, semaphore_name, NULL);
                perror("execlp error\n");
                exit(EXECLP_ERROR); 
                
            default: //macierzysty, obsluga funkcjonalnosci Ctr-c
                if (signal(SIGINT, del_sem) == SIG_ERR) 
                {
                    perror("signal error"); 
                    exit(SIGNAL_ERROR);
                }
                sleep(1);
        }
    }

    if (signal(SIGINT, del_sem) == SIG_ERR) //obsluga funkcjonalnosci Ctr-c podczas oczekiwania na potomne
    {
        perror("signal error"); 
        exit(SIGNAL_ERROR);
    }    
    for (int i = 0; i < num_processes; i++) 
    {
        if (waitpid(-1, NULL, 0) == -1) 
        {
            perror("wait error"); 
            exit(WAITPID_ERROR); 
        }
    }

    char file_content[FILE_CONTENT_SIZE];
    file = fopen("numer.txt", "r");
    if (file == NULL) 
    {
        perror("fopen error");
        exit(FOPEN_ERROR);
    }
    if (fscanf(file, "%s", file_content) < 1) 
    {
        perror("fscanf error");
        exit(FSCANF_ERROR);
    }
    my_fclose(file);

    int num1 = strtol(file_content, NULL, 0);
    int num2 = num_processes*atoi(num_sections);
    if(num1 == num2)
        printf("Numer z pliku (%d) jest zgodny z oczekiwanym wynikiem (%d)\n", num1, num2);
    else
        printf("Numer z pliku (%d) NIE jest zgodny z oczekiwanym wynikiem (%d)\n", num1, num2);

    atexit(del_sem);

    return 0;
}
