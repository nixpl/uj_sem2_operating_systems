#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define K_BUFSIZE 2
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
        perror("close fout error");
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

    int fout, fifo, nbytes;
    char buf[K_BUFSIZE];

    fout = open(argv[1], O_WRONLY | O_CREAT, 0644); 
    if (fout == -1)
    {
        perror("creat error");
        exit(CREAT_ERROR);
    }

    fifo = open(argv[2], O_RDONLY, 0644);
    if(fifo == -1)
    {
        perror("fifo opem error");
        exit(OPEN_ERROR);
    }

    while ((nbytes = read(fifo, buf, K_BUFSIZE)) > 0)
    {
        char txt[TXT_LEN];
        // printf("Konsument pobral %d bajtow z potoku: %.*s\n", nbytes, nbytes, buf);
        if(snprintf(txt, TXT_LEN, "Konsument pobral %d bajtow z potoku: %.*s\n", nbytes, nbytes, buf) < 0)
        {
            perror("snprintf error");
            exit(SNPRINTF_ERROR);
        }
        my_write(STDOUT_FILENO, txt, strlen(txt));

        my_write(fout, buf, nbytes);
    
        // printf("Konsument umiescil %d bajtow w pliku\n", nbytes);
        if(snprintf(txt, TXT_LEN, "Konsument umiescil %d bajtow w pliku\n", nbytes) < 0)
        {
            perror("snprintf error");
            exit(SNPRINTF_ERROR);
        }
        my_write(STDOUT_FILENO, txt, strlen(txt));

        sleep(rand() % 3); // losowe przetwarzanie towaru
    }

    my_close(fout);
    my_close(fifo);

    

    return 0;
}