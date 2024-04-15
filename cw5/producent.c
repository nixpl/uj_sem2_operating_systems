#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define P_BUFSIZE 3
#define TXT_LEN 100

#define CREAT_ERROR 1
#define WRITE_ERROR 2
#define OPEN_ERROR 3
#define PIPE_ERROR 4
#define FORK_ERROR 5
#define CLOSE_ERROR 6
#define ARGC_ERROR 7
#define SNPRINTF_ERROR 8

void my_close(int f)
{
    if(close(f) == -1)
    {
        perror("close fin error");
        exit(CLOSE_ERROR);
    }
    return;
}

void my_write(int fileds, char buf[], int nbytes)
{
    if (write(fileds, buf, nbytes) != nbytes)
        {
            perror("write error");
            exit(WRITE_ERROR);
        }
    return;
}

int main(int argc, char *argv[])
{
    

    if(argc != 3)
    {
        printf("podano %d argumentow zamiast 2\n", argc-1);
        exit(ARGC_ERROR);
    }

    int fin, fifo, nbytes;
    char buf[P_BUFSIZE];

    fin = open(argv[1], O_RDONLY, 0644);
    if (fin == -1)
    {
        perror("open error");
        exit(OPEN_ERROR);
    }

    fifo = open(argv[2], O_WRONLY, 0644);
    if(fifo == -1)
    {
        perror("fifo opem error");
        exit(OPEN_ERROR);
    }

    while ((nbytes = read(fin, buf, P_BUFSIZE)) > 0)
    {
        char txt[TXT_LEN];
        // printf("Producent pobral %d bajtow z pliku: %.*s\n", nbytes, nbytes, buf);
        if(snprintf(txt, TXT_LEN, "Producent pobral %d bajtow z potoku: %.*s\n", nbytes, nbytes, buf) < 0)
        {
            perror("snprintf error");
            exit(SNPRINTF_ERROR);
        }
        my_write(STDOUT_FILENO, txt, strlen(txt));

        my_write(fifo, buf, nbytes);
    
        // printf("Producent umiescil %d bajtow w potoku\n", nbytes);
        if(snprintf(txt, TXT_LEN, "Producent umiescil %d bajtow w potoku\n", nbytes) < 0)
        {
            perror("snprintf error");
            exit(SNPRINTF_ERROR);
        }
        my_write(STDOUT_FILENO, txt, strlen(txt));

        sleep(rand() % 5); // losowa produkcja towaru
    }

    my_close(fin);
    my_close(fifo);

    


    return 0;
}