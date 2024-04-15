#include <stdio.h>
#include <wait.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>

#include "semaphore_fun.h"
#include "shared_memory_fun.h"

enum ERRORS 
{
    ARGC_ERROR = 1, 
    SIGNAL_ERROR, 
    EXECLP_ERROR,  
    ATEXIT_ERROR, 
};

#define TRUE 1
#define FALSE 0

#define NELE 20
#define NBUF 5

#define PROD_SEMAPHORE_NAME "prod_semaphore"
#define CONS_SEMAPHORE_NAME "cons_semaphore"
#define MEMORY_DESCRIPTOR_NAME "my_memory"

typedef struct
{
    char bufor[NBUF][NELE];
    int wstaw; 
    int wyjmij;
} SegmentPD;

int prod_exists = FALSE;
int cons_exists = FALSE;
int memory_exists = FALSE;

void deleting(void)
{
    if(prod_exists == TRUE)
        delete_semaphore(PROD_SEMAPHORE_NAME);
    if(cons_exists == TRUE)
        delete_semaphore(CONS_SEMAPHORE_NAME);
    if(memory_exists == TRUE)
        delete_shared_memory(MEMORY_DESCRIPTOR_NAME);
}

void deleting2(int signum)
{
    if(prod_exists == TRUE)
        delete_semaphore(PROD_SEMAPHORE_NAME);
    if(cons_exists == TRUE)
        delete_semaphore(CONS_SEMAPHORE_NAME);
    if(memory_exists == TRUE)
        delete_shared_memory(MEMORY_DESCRIPTOR_NAME);
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
            if(execlp(runname, program_name, argv, PROD_SEMAPHORE_NAME, 
                CONS_SEMAPHORE_NAME, MEMORY_DESCRIPTOR_NAME, NULL) == -1)
            {
                perror("execlp error");
                exit(EXECLP_ERROR);
            }
            break;
        default:
            break;
    }
    return pid;
}

int main(int argc, char *argv[])
{
    if(argc!=5)
    {
        printf("Podano %d argumentow zamiast 4\n", argc-1);
        printf("Spodziewano sie: <producent_prog_name> <consument_prog_name> <input_file_name> <output_file_name>\n");
        exit(ARGC_ERROR);
    }

    if(atexit(deleting) != 0)
    {
        perror("atexit error");
        exit(ATEXIT_ERROR);
    }

    create_semaphore(PROD_SEMAPHORE_NAME, O_CREAT | O_EXCL, 0666, NBUF);
    sem_t* producent_adress = open_semaphore(PROD_SEMAPHORE_NAME, 0);
    prod_exists = TRUE;
    printf("Utworzono: %s[adres: %p, wartosc: %d]\n", PROD_SEMAPHORE_NAME, 
        (void *)producent_adress, get_semaphore_value(producent_adress));



    create_semaphore(CONS_SEMAPHORE_NAME, O_CREAT | O_EXCL, 0666, 0);
    sem_t* consument_adress = open_semaphore(CONS_SEMAPHORE_NAME, 0);
    cons_exists = TRUE;
    printf("\nUtworzono: %s[adres: %p, wartosc: %d]\n", CONS_SEMAPHORE_NAME, 
        (void *)consument_adress, get_semaphore_value(consument_adress));



    create_shared_memory(MEMORY_DESCRIPTOR_NAME, sizeof(SegmentPD));
    int memory_descriptor = open_shared_memory(MEMORY_DESCRIPTOR_NAME);
    memory_exists = TRUE;
    printf("\nUtworzono: %s[deskryptor: %d, rozmiar: %ld]\n", MEMORY_DESCRIPTOR_NAME, 
        memory_descriptor, sizeof(SegmentPD));



    if(signal(SIGINT, deleting2) == SIG_ERR)
    {
        perror("signal error\n");
        exit(SIGNAL_ERROR);
    }

    run_child(argv[1], argv[3]); // "producent.x", "surowiec.txt"
    run_child(argv[2], argv[4]); // "konsument.x", "towar.txt"

    wait(NULL);    
    wait(NULL);

    close_semaphore(producent_adress);
    close_semaphore(consument_adress);
    close_shared_memory(memory_descriptor);
    
    return 0;
}