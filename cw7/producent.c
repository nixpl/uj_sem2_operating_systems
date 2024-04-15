#include <stdio.h>
#include <wait.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

#include "semaphore_fun.h"
#include "shared_memory_fun.h"

#define TRUE 1
#define FALSE 0

enum ERRORS
{
    READ_ERROR = 1,
    OPEN_ERROR,
    CLOSE_ERROR,
    ARGC_ERROR
};

#define NELE 20
#define NBUF 5

typedef struct
{
    char bufor[NBUF][NELE];
    int wstaw; 
    int wyjmij;
} SegmentPD;

int my_read(int fileds, char buf[], int nbytes)
{
    int how_many_bytes = read(fileds, buf, nbytes);
    if(how_many_bytes == -1)
    {
        perror("read error");
        exit(READ_ERROR);
    }
    return how_many_bytes;
}

int main(int argc, char *argv[])
{
    if(argc != 5)
    {
        printf("podano %d argumentow zamiast 2\n", argc-1);
        printf("Spodziewano sie: <input_file_name> <producent_semaphore_name> <consument_semaphore_name> <memory_descriptor_name>\n");
        exit(ARGC_ERROR);
    }

    int file = open(argv[1], O_RDONLY, 0666);
    if(file == -1)
    {
        perror("open error");
        exit(OPEN_ERROR);
    }
  
    sem_t* producent_adress = open_semaphore(argv[2], 0);
    sem_t* consument_adress = open_semaphore(argv[3], 0);
    int memory_descriptor = open_shared_memory(argv[4]);
    printf("\nProducent -> semafor: %s[adres: %p, wartosc: %d]\n", argv[2], 
        (void *)producent_adress, get_semaphore_value(producent_adress));

    SegmentPD* addr = (SegmentPD*)map_shared_memory(sizeof(SegmentPD), memory_descriptor);
    addr->wstaw = 0;

    int repeat = TRUE;
    while(repeat)
    {
        acquire_semaphore(producent_adress);

        int how_many_bytes = my_read(file, addr->bufor[addr->wstaw], sizeof(addr->bufor[addr->wstaw]));
        if(how_many_bytes == NELE)
        {
            printf("\nProducent ->   [wartosc semafora producenta: %d]\n               [wartosc semafora konsumenta: %d]\n               [indeks buforu: %d, rozmiar: %dB]\n               [tekst: %.*s]\n", 
            get_semaphore_value(producent_adress), get_semaphore_value(consument_adress), 
            addr->wstaw, NELE, NELE, addr->bufor[addr->wstaw]);

            addr->wstaw = (addr->wstaw + 1) % NBUF;
        }
        else if(how_many_bytes >= 0)
        {
            printf("\nProducent ->   [wartosc semafora producenta: %d]\n               [wartosc semafora konsumenta: %d]\n               [indeks buforu: %d, rozmiar: %dB]\n               [tekst: %.*s]\n", 
            get_semaphore_value(producent_adress), get_semaphore_value(consument_adress), 
            addr->wstaw, how_many_bytes, how_many_bytes, addr->bufor[addr->wstaw]);

            addr->bufor[addr->wstaw][how_many_bytes] = '\0';

            repeat = FALSE;
        }
        release_semaphore(consument_adress);

        sleep(rand() % 5); // losowe przetwarzanie towaru
    }

    close_semaphore(producent_adress);
    close_semaphore(consument_adress);
    unmap_shared_memory(addr, sizeof(SegmentPD));
    close_shared_memory(file);

    return 0;
}