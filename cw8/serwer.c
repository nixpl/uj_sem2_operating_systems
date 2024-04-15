#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#include "msg_q_fun.h"

#define TRUE 1
#define FALSE 0
#define MSG_LEN 100

enum ERRORS 
{
    ATEXIT_ERROR = 10,
    SIGNAL_ERROR,
    SPRINTF_ERROR,
    SSCANF_ERROR
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
        msg_q_delete(SERWER_NAME);
    }
}

void exiting(int signum)
{
    printf("Odebrano sygnal przerwania\n"); 
    msg_q_close(client_mqdes);
    msg_q_close(server_mqdes);
    msg_q_delete(SERWER_NAME); 
    normal_close = TRUE;  
    exit(0);
}


int main()
{
    if(atexit(deleting) == -1)
    {
        perror("atexit error");
        exit(ATEXIT_ERROR);
    }

    if(signal(SIGINT, exiting) == SIG_ERR)
    {
        perror("signal error");
        exit(SIGNAL_ERROR);
    }

    struct mq_attr newattr;
    newattr.mq_flags = 0;
    newattr.mq_maxmsg = 10;
    newattr.mq_msgsize = MSG_LEN;
    newattr.mq_curmsgs = 0;

    server_mqdes = msg_q_create(SERWER_NAME, &newattr);
    server_mqdes = msg_q_open(SERWER_NAME, O_RDONLY);

    struct mq_attr attr;
    msg_q_getattr(server_mqdes, &attr);
    printf("Stworzono kolejke %s [deskryptor: %d]\n", SERWER_NAME, server_mqdes);
    printf("-> mq_flags   = %ld\n", attr.mq_flags);
    printf("-> mq_maxmsg  = %ld\n", attr.mq_maxmsg);
    printf("-> mq_msgsize = %ld\n", attr.mq_msgsize);
    printf("-> mq_curmsgs = %ld\n", attr.mq_curmsgs);

    char msg_send[MSG_LEN];
    char msg_received[MSG_LEN];
    char operator;
    int num1, num2;
    int pid;
    while(TRUE)
    {
        printf("Oczekiwanie na polecenie...\n");

        msg_q_receive(server_mqdes, msg_received, MSG_LEN);
        printf("PID klienta wraz z otrzymanym poleceniem: %s\n", msg_received);

        if(sscanf(msg_received, "%d %d%c%d", &pid, &num1, &operator, &num2) != 4)
        {
            perror("sscanf error");
            exit(SSCANF_ERROR);
        }

        if(sprintf(client_name, "/%d", pid) < 0)
        {
            perror("sprintf error");
            exit(SPRINTF_ERROR);
        }

        printf("trwa obliczanie...\n");

        sleep(rand()%5);        

        client_mqdes = msg_q_open(client_name, O_WRONLY);
        switch (operator)
        {
            case '+':
                sprintf(msg_send, "%d", num1 + num2);
                break;
            case '-':
                sprintf(msg_send, "%d", num1 - num2);
                break;
            case '*':
                sprintf(msg_send, "%d", num1 * num2);
                break;
            case '/':
                if(num2 != 0)
                    sprintf(msg_send, "%d", num1 / num2);
                else
                    sprintf(msg_send, "Dzielisz przez 0");
                break;
            default:
                sprintf(msg_send, "Nieznany operator");
                break;
        }

        msg_q_send(client_mqdes, msg_send, MSG_LEN, 0);
        printf("Wynik zostal pomyslnie wyslany do klienta [PID: %d]\n", pid);
        msg_q_close(client_mqdes);
    }
    
    return 0;
}