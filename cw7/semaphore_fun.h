#ifndef SEMAPHORE_FUN_H
#define SEMAPHORE_FUN_H

#include <semaphore.h>


sem_t* create_semaphore(const char* name, int oflag, mode_t mode, unsigned int value);
sem_t* open_semaphore(const char* name, int oflag);
int get_semaphore_value(sem_t* semaphore);
void release_semaphore(sem_t* semaphore);
void acquire_semaphore(sem_t* semaphore);
void close_semaphore(sem_t* semaphore);
void delete_semaphore(const char* name);

#endif /* SEMAPHORE_FUN_H */