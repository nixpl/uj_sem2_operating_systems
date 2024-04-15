#ifndef SEMAPHORE_FUN_H
#define SEMAPHORE_FUN_H

#include <semaphore.h>

typedef sem_t* semaphore_t;

sem_t* create_semaphore(const char* name, int oflag, mode_t mode, unsigned int value);
sem_t* open_semaphore(const char* name, int oflag);
int get_semaphore_value(semaphore_t semaphore);
void release_semaphore(semaphore_t semaphore);
void acquire_semaphore(semaphore_t semaphore);
void close_semaphore(semaphore_t semaphore);
void delete_semaphore(const char* name);

#endif /* SEMAPHORE_FUN_H */