#include "msg_q_fun.h"

enum ERRORS 
{
    MQ_OPEN_ERROR = 1,
    MQ_CLOSE_ERROR,
    MQ_UNLINK_ERROR,
    MQ_SEND_ERROR,
    MQ_RECEIVE_ERROR,
    MQ_GETATTR_ERROR,
    MQ_SETATTR_ERROR
};


mqd_t msg_q_create(const char* name, struct mq_attr* attr)
{
    mqd_t mqdes = mq_open(name, O_CREAT | O_EXCL | O_RDWR, 0666, attr);
    if(mqdes == -1)
    {
        perror("mq_open error");
        exit(MQ_OPEN_ERROR);
    }
    return mqdes;
}

mqd_t msg_q_open(const char* name, int flag)
{
    mqd_t mqdes = mq_open(name, flag);
    if(mqdes == -1)
    {
        perror("mq_open error");
        exit(MQ_OPEN_ERROR);
    }
    return mqdes;

}

void msg_q_close(mqd_t mqdes)
{
    if(mq_close(mqdes) == -1)
    {
        perror("mq_close error");
        exit(MQ_CLOSE_ERROR);
    }
}

void msg_q_delete(const char* name)
{
    if (mq_unlink(name) == -1)
    {
        perror("mq_unlink error");
        exit(MQ_UNLINK_ERROR);
    }
}

void msg_q_send(mqd_t mqdes, char* buffor, int size, int prio)
{
    if (mq_send(mqdes, buffor, size, prio) == -1)
    {
        perror("mq_send error");
        exit(MQ_SEND_ERROR);
    }
}

void msg_q_receive(mqd_t mqdes, char* buffor, int size)
{
    if (mq_receive(mqdes, buffor, size, NULL) == -1)
    {
        perror("mq_receive error");
        exit(MQ_RECEIVE_ERROR);
    }
}

void msg_q_getattr(mqd_t mqdes, struct mq_attr* attr)
{
    if(mq_getattr(mqdes, attr) == -1)
    {
        perror("mq_getattr error");
        exit(MQ_GETATTR_ERROR);
    }
}

void msg_q_setattr(mqd_t mqdes, struct mq_attr* newattr, struct mq_attr* oldattr)
{
    if (mq_setattr(mqdes, newattr, oldattr) == -1)
    {
        perror("mq_setattr error");
        exit(MQ_SETATTR_ERROR);
    }
}