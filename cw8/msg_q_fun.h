#ifndef MSG_Q_H
#define MSG_Q_H

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>

#define SERWER_NAME "/my_server"


mqd_t msg_q_create(const char* name, struct mq_attr* attr);

mqd_t msg_q_open(const char* name, int flag);

void msg_q_close(mqd_t mqdes);

void msg_q_delete(const char* name);

void msg_q_send(mqd_t mqdes, char* buffor, int size, int prio);

void msg_q_receive(mqd_t mqdes, char* buffor, int size);

void msg_q_getattr(mqd_t mqdes, struct mq_attr* attr);

void msg_q_setattr(mqd_t mqdes, struct mq_attr* newattr, struct mq_attr* oldattr);

#endif