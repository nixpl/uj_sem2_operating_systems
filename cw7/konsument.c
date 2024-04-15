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
    WRITE_ERROR = 1,
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

void my_write(int fileds, char buf[], int nbytes)
{
    int i;
    for (i = nbytes - 1; i >= 0; i--)
    {
        if (buf[i] != '\0')
        {
            break;
        }
    }
    nbytes = i + 1;

    if(write(fileds, buf, nbytes) != nbytes)
        {
            perror("write error");
            exit(WRITE_ERROR);
        }
    return;
}

int end_position_index(SegmentPD addr, int nbuf)
{
    for(int i = 0; i < NELE; i++)
        if(addr.bufor[nbuf][i] == '\0')
            return i;
    return -1;
}

int main(int argc, char *argv[])
{    
    if(argc != 5)
    {
        printf("Podano %d argumentow zamiast 4\n", argc-1);
        printf("Spodziewano sie: <output_file_name> <producent_semaphore_name> <consument_semaphore_name> <memory_descriptor_name>\n");
        exit(ARGC_ERROR);
    }

    int file = open(argv[1], O_RDWR | O_TRUNC, 0666);
    if(file == -1)
    {
        perror("open error");
        exit(OPEN_ERROR);
    }

    sem_t* producent_adress = open_semaphore(argv[2], 0);
    sem_t* consument_adress = open_semaphore(argv[3], 0);
    int memory_descriptor = open_shared_memory(argv[4]);
    printf("\nKonsument -> semafor: %s[adres: %p, wartosc: %d]\n", argv[3], 
        (void *)consument_adress, get_semaphore_value(consument_adress));

    SegmentPD* addr = (SegmentPD*)map_shared_memory(sizeof(SegmentPD), memory_descriptor);
    addr->wyjmij = 0;


    int repeat = TRUE;
    while(repeat)
    {
        acquire_semaphore(consument_adress);

        int end_pos = end_position_index(*addr, addr->wyjmij);
        if(end_pos == -1)
        {
            printf("\nKonsument ->   [wartosc semafora producenta: %d]\n               [wartosc semafora konsumenta: %d]\n               [indeks buforu: %d, rozmiar: %dB]\n               [tekst: %.*s]\n", 
            get_semaphore_value(producent_adress), get_semaphore_value(consument_adress), 
            addr->wyjmij, NELE,  NELE, addr->bufor[addr->wyjmij]);

            my_write(file, addr->bufor[addr->wyjmij], sizeof(addr->bufor[addr->wyjmij]));

            addr->wyjmij = (addr->wyjmij + 1) % NBUF;
        }
        else
        {
            printf("\nKonsument ->   [wartosc semafora producenta: %d]\n               [wartosc semafora konsumenta: %d]\n               [indeks buforu: %d, rozmiar: %dB]\n               [tekst: %.*s]\n", 
            get_semaphore_value(producent_adress), get_semaphore_value(consument_adress), 
            addr->wyjmij, NELE,  end_pos, addr->bufor[addr->wyjmij]);

            my_write(file, addr->bufor[addr->wyjmij], sizeof(addr->bufor[addr->wyjmij]));

            repeat = FALSE;
        }        

        release_semaphore(producent_adress);

        sleep(rand() % 3); // losowe przetwarzanie towaru
    }

    close_semaphore(producent_adress);
    close_semaphore(consument_adress);
    unmap_shared_memory(addr, sizeof(SegmentPD));
    close_shared_memory(file);

    return 0;
}