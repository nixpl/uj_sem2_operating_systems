#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

#define K_BUFSIZE 2
#define P_BUFSIZE 3
#define TXT_LEN 100

#define CREAT_ERROR 1
#define WRITE_ERROR 2
#define OPEN_ERROR 3
#define PIPE_ERROR 4
#define FORK_ERROR 5
#define CLOSE_ERROR 6
#define SNPRINTF_ERROR 7

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

void konsument(int fd)
{
    int fout, nbytes;
    char buf[K_BUFSIZE];

    fout = creat("towar.txt", 0644); // uprawnienia: wlasciciel odczyt i zapis, grupa odczyt
    if (fout == -1)
    {
        perror("creat error");
        exit(CREAT_ERROR);
    }

    while ((nbytes = read(fd, buf, K_BUFSIZE)) > 0)
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
    my_close(fd);
    return;
}

void producent(int fd)
{
    int fin, nbytes;
    char buf[P_BUFSIZE];

    fin = open("surowiec.txt", O_RDONLY);
    if (fin == -1)
    {
        perror("open error");
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

        my_write(fd, buf, nbytes);
        
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
    my_close(fd);
    return;
}

int main(int argc, char *argv[])
{
    int p[2], status;
    pid_t pid;
    srand(time(NULL));

    if (pipe(p) == -1)
    {
        perror("pipe error");
        exit(PIPE_ERROR);
    }

    pid = fork();
    switch (pid)
    {
        case -1:
            perror("fork error");
            exit(FORK_ERROR);

        case 0:          // konsument
            close(p[1]); // zamknięcie końcówki piszącej
            konsument(p[0]);
            exit(0);

        default:         // producent
            close(p[0]); // zamknięcie końcówki czytającej
            producent(p[1]);
            close(p[1]);
            waitpid(pid, &status, 0); // czekanie na proces potomny
    }

    return 0;
}