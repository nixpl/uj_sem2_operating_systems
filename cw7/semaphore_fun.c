#include "semaphore_fun.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>

#define SEM_OPEN_ERROR 1
#define SEM_GETVALUE_ERROR 2
#define SEM_POST_ERROR 3
#define SEM_WAIT_ERROR 4
#define SEM_CLOSE_ERROR 5
#define SEM_UNLINK_ERROR 6

sem_t* create_semaphore(const char* name, int oflag, mode_t mode, unsigned int value) 
{
    sem_t* semaphore = sem_open(name, oflag, mode, value);
    if(semaphore == SEM_FAILED) 
    {
        perror("sem_open error");
        exit(SEM_OPEN_ERROR);
    }
    return semaphore;
}

sem_t* open_semaphore(const char* name, int oflag) 
{
    sem_t* semaphore = sem_open(name, oflag);
    if(semaphore == SEM_FAILED) 
    {
        perror("sem_open error");
        exit(SEM_OPEN_ERROR);
    }
    return semaphore;
}

int get_semaphore_value(sem_t* semaphore) 
{
    int value;
    if(sem_getvalue(semaphore, &value) == -1) 
    {
        perror("sem_getvalue error");
        exit(SEM_GETVALUE_ERROR);
    }
    return value;
}

void release_semaphore(sem_t* semaphore) 
{
    if(sem_post(semaphore) == -1) 
    {
        perror("sem_post error");
        exit(SEM_POST_ERROR);
    }
}

void acquire_semaphore(sem_t* semaphore) 
{
    if(sem_wait(semaphore) == -1) 
    {
        perror("sem_wait error");
        exit(SEM_WAIT_ERROR);
    }
}

void close_semaphore(sem_t* semaphore) 
{
    if(sem_close(semaphore) == -1) 
    {
        perror("sem_close error");
        exit(SEM_CLOSE_ERROR);
    }
}

void delete_semaphore(const char* name) 
{
    if(sem_unlink(name) == -1) 
    {
        perror("sem_unlink error");
        exit(SEM_UNLINK_ERROR);
    }
}
