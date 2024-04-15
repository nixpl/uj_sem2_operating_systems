#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#include "msg_q_fun.h"

#define MSG_LEN 100
#define TRUE 1
#define FALSE 0

enum ERRORS 
{
    ATEXIT_ERROR = 10,
    SIGNAL_ERROR,
    SPRINTF_ERROR
};

mqd_t client_mqdes;
mqd_t server_mqdes;

char client_name[50];

int normal_close = FALSE;

void deleting(void)
{
    if(normal_close == FALSE)
    {
        msg_q_close(client_mqdes);
        msg_q_close(server_mqdes);
        msg_q_delete(client_name);
    }
}

void exiting(int signum)
{
    printf("Odebrano sygnal przerwania\n");
    msg_q_close(client_mqdes);
    msg_q_close(server_mqdes);
    msg_q_delete(client_name);
    normal_close = TRUE;
    exit(0);
}

int pid_len(pid_t pid)
{
    int p = (int)pid;
    int len = 1;
    while(p /= 10)
        len++;
    
    return len;
}

int main()
{
    if(atexit(deleting) != 0)
    {
        perror("atexit error");
        exit(ATEXIT_ERROR);
    }

    if(signal(SIGINT, exiting) == SIG_ERR)
    {
        perror("signal error");
        exit(SIGNAL_ERROR);
    }

    pid_t pid = getpid();
    sprintf(client_name, "/%d", (int)pid);

    struct mq_attr newattr;
    newattr.mq_flags = 0;
    newattr.mq_maxmsg = 10;
    newattr.mq_msgsize = MSG_LEN;
    newattr.mq_curmsgs = 0;

    client_mqdes = msg_q_create(client_name, &newattr);
    client_mqdes = msg_q_open(client_name, O_RDONLY);
    server_mqdes = msg_q_open(SERWER_NAME, O_WRONLY);

    struct mq_attr attr;
    msg_q_getattr(client_mqdes, &attr);
    printf("Stworzono kolejke %s [deskryptor: %d]\n", SERWER_NAME, client_mqdes);
    printf("-> mq_flags   = %ld\n", attr.mq_flags);
    printf("-> mq_maxmsg  = %ld\n", attr.mq_maxmsg);
    printf("-> mq_msgsize = %ld\n", attr.mq_msgsize);
    printf("-> mq_curmsgs = %ld\n", attr.mq_curmsgs);

    char read_buff[MSG_LEN];
    char msg_send[MSG_LEN + pid_len(pid) + 2];
    char msg_received[MSG_LEN];
    printf("Podaj dzialanie (bez spacji) w formacie <liczba><operator><liczba>: ");
    while(fgets(read_buff, sizeof(read_buff), stdin) != NULL)
    {
        printf("%s\n", read_buff);
        if(sprintf(msg_send, "%d %s", (int)pid, read_buff) < 0)
        {
            perror("sprintf error");
            exit(SPRINTF_ERROR);
        }
        printf("%s\n", msg_send);

        msg_q_send(server_mqdes, msg_send, MSG_LEN, 0);
        printf("Wiadomosc pomyslnie wyslana\n");

        sleep(rand()%5);

        msg_q_receive(client_mqdes, msg_received, MSG_LEN);
        printf("Odpowiedź: %s = %s\n", read_buff, msg_received);

        printf("Podaj dzialanie (bez spacji) w formacie <liczba><operator><liczba>: ");
    }

    return 0;
}