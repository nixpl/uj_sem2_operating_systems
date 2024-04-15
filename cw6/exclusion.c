#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>

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
#define SPRINTF_ERROR 10

#define FILE_CONTENT_SIZE 20

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
    if (argc != 3) 
    {
        printf("Zla ilosc argumentow (%d), spodziewano sie 2\n", argc);
        exit(WRONG_ARG_NUM);
    }

    int num_sections = strtol(argv[1], NULL, 0);
    char* semaphore_name =argv[2];
    sem_t* semaphore = open_semaphore(semaphore_name, 0);

    for(int i=0; i <num_sections; i++)
    {
        printf("    %d sekcja krytyczna                             [PID: %d]\n", i+1, getpid());
        printf("Wartosc semafora przed czescia krytyczna: %d        [PID: %d]\n", get_semaphore_value(semaphore), getpid());
        sleep(rand() % 3);
        
        acquire_semaphore(semaphore);

        char file_content[FILE_CONTENT_SIZE];
        FILE *file = fopen("numer.txt", "r");
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

        int num = strtol(file_content, NULL, 0);
        printf("    Numer z pliku (%d)                              [PID: %d]\n", num, getpid());
        sleep(rand() % 3);
        
        if(sprintf(file_content, "%d", ++num) < 0)
        {
            perror("sprintf error");
            exit(SPRINTF_ERROR);
        }
        
        file = fopen("numer.txt", "w");
        if (file == NULL) 
        {
            perror("fopen error");
            exit(FOPEN_ERROR);
        }
        if (fprintf(file, "%s", file_content) < 0) 
        {
            perror("fprintf error");
            my_fclose(file);
            exit(FPRINTF_ERROR);
        }
        my_fclose(file);
              
        printf("    Wartosc semafora w czesci krytycznej: %d        [PID: %d]\n", get_semaphore_value(semaphore), getpid());

        release_semaphore(semaphore);
        
        printf("Wartosc semafora po czesci krytycznej: %d           [PID: %d]\n", get_semaphore_value(semaphore), getpid());
    }

    return 0;
}